// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/WitchPTEquipmentManagerComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Engine/ActorChannel.h"
#include "Equipment/WitchPTEquipmentDefinition.h"
#include "Equipment/WitchPTEquipmentInstance.h"
#include "Inventory/WitchPTInventoryItemDefinition.h"
#include "Inventory/WitchPTInventoryItemInstance.h"
#include "Inventory/Fragments/WitchPTInventoryItemFragment_EquippableItem.h"
#include "Net/UnrealNetwork.h"


void FWitchPTEquipmentList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	UWitchPTEquipmentManagerComponent* EquipmentManager = Cast<UWitchPTEquipmentManagerComponent>(OwnerComponent);
	if (!IsValid(EquipmentManager)) return;
	
	for (int32 Index : RemovedIndices)
	{
		const FWitchPTEquipmentEntry& Entry = Entries[Index];
		if (Entry.Instance != nullptr)
		{
			Entry.Instance->OnUnequipped();
			EquipmentManager->OnItemUnequipped.Broadcast(Entry.Instance);
		}
	}
}

void FWitchPTEquipmentList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	UWitchPTEquipmentManagerComponent* EquipmentManager = Cast<UWitchPTEquipmentManagerComponent>(OwnerComponent);
	if (!IsValid(EquipmentManager)) return;
	
	for (int32 Index : AddedIndices)
	{
		const FWitchPTEquipmentEntry& Entry = Entries[Index];
		if (Entry.Instance != nullptr)
		{
			Entry.Instance->OnEquipped();
			EquipmentManager->OnItemEquipped.Broadcast(Entry.Instance);
		}
	}
}

void FWitchPTEquipmentList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
}

UWitchPTEquipmentInstance* FWitchPTEquipmentList::AddEntry(TSubclassOf<UWitchPTEquipmentDefinition> EquipmentDefinition)
{
	UWitchPTEquipmentInstance* Result = nullptr;

	check(EquipmentDefinition != nullptr);
	check(OwnerComponent);
	check(OwnerComponent->GetOwner()->HasAuthority());
    
	const UWitchPTEquipmentDefinition* EquipmentCDO = GetDefault<UWitchPTEquipmentDefinition>(EquipmentDefinition);

	TSubclassOf<UWitchPTEquipmentInstance> InstanceType = EquipmentCDO->InstanceType;
	if (InstanceType == nullptr)
	{
		InstanceType = UWitchPTEquipmentInstance::StaticClass();
	}
    
	FWitchPTEquipmentEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.EquipmentDefinition = EquipmentDefinition;
	NewEntry.Instance = NewObject<UWitchPTEquipmentInstance>(OwnerComponent->GetOwner(), InstanceType);
	Result = NewEntry.Instance;

	// Grant abilities
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		for (TSubclassOf<UGameplayAbility> AbilityClass : EquipmentCDO->AbilitiesToGrant)
		{
			if (AbilityClass)
			{
				FGameplayAbilitySpec AbilitySpec(AbilityClass, 1, INDEX_NONE, Result);
				FGameplayAbilitySpecHandle Handle = ASC->GiveAbility(AbilitySpec);
				NewEntry.AbilityHandles.GrantedHandles.Add(Handle);
				UE_LOG(LogTemp, Warning, TEXT("🎯 Granted ability: %s"), *AbilityClass->GetName());
			}
		}
	}
	Result->SpawnEquipmentActors(EquipmentCDO->ActorsToSpawn);

	MarkItemDirty(NewEntry);
	return Result;
}

void FWitchPTEquipmentList::RemoveEntry(UWitchPTEquipmentInstance* Instance)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FWitchPTEquipmentEntry& Entry = *EntryIt;
		if (Entry.Instance == Instance)
		{
			// Remove abilities
			if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
			{
				for (const FGameplayAbilitySpecHandle& Handle : Entry.AbilityHandles.GrantedHandles)
				{
					if (Handle.IsValid())
					{
						ASC->ClearAbility(Handle);
						UE_LOG(LogTemp, Warning, TEXT("🚫 Removed ability"));
					}
				}
			}

			EntryIt.RemoveCurrent();
			Instance->DestroyEquipmentActors();
			MarkArrayDirty();
		}
	}
}

UAbilitySystemComponent* FWitchPTEquipmentList::GetAbilitySystemComponent() const
{
	check(OwnerComponent);
	AActor* OwningActor = OwnerComponent->GetOwner();
	return Cast<UAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor));
}

//-------------------------------      UWitchPTEquipmentManagerComponent           -----------------------//
// -------------------------------------------------------------------------------------------------------//
UWitchPTEquipmentManagerComponent::UWitchPTEquipmentManagerComponent()
	: EquipmentList(this)
{
	SetIsReplicatedByDefault(true);
	bReplicateUsingRegisteredSubObjectList = true;
}

void UWitchPTEquipmentManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, EquipmentList);
}

bool UWitchPTEquipmentManagerComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch,
	FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (FWitchPTEquipmentEntry& Entry : EquipmentList.Entries)
	{
		UWitchPTEquipmentInstance* Instance = Entry.Instance;
		if (IsValid(Instance))
		{
			WroteSomething |= Channel->ReplicateSubobject(Instance, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}

void UWitchPTEquipmentManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UWitchPTEquipmentManagerComponent::UninitializeComponent()
{
	TArray<UWitchPTEquipmentInstance*> AllEquipmentInstances;

	// Gather all instances before removal to avoid side effects affecting the equipment list iterator    
	for (const FWitchPTEquipmentEntry& Entry : EquipmentList.Entries)
	{
		AllEquipmentInstances.Add(Entry.Instance);
	}

	for (UWitchPTEquipmentInstance* EquipInstance : AllEquipmentInstances)
	{
		UnequipItem(EquipInstance);
	}

	Super::UninitializeComponent();
}

void UWitchPTEquipmentManagerComponent::ReadyForReplication()
{
	Super::ReadyForReplication();
	// Register existing equipment instances
	if (IsUsingRegisteredSubObjectList())
	{
		for (const FWitchPTEquipmentEntry& Entry : EquipmentList.Entries)
		{
			UWitchPTEquipmentInstance* Instance = Entry.Instance;
			if (IsValid(Instance))
			{
				AddReplicatedSubObject(Instance);
			}
		}
	}
}

void UWitchPTEquipmentManagerComponent::EquipItem(TSubclassOf<UWitchPTEquipmentDefinition> EquipmentDefinition)
{
	if (GetOwner()->GetLocalRole() < ROLE_Authority)
	{
		Server_EquipItem(EquipmentDefinition);
	}
	else
	{
		if (EquipmentDefinition != nullptr)
		{
			UWitchPTEquipmentInstance* NewInstance = EquipmentList.AddEntry(EquipmentDefinition);
			if (NewInstance != nullptr)
			{
				NewInstance->OnEquipped();
				OnItemEquipped.Broadcast(NewInstance);

				if (IsUsingRegisteredSubObjectList())
				{
					AddReplicatedSubObject(NewInstance);
				}
			}
		}
	}
}

void UWitchPTEquipmentManagerComponent::Server_EquipItem_Implementation(TSubclassOf<UWitchPTEquipmentDefinition> EquipmentDefinition)
{
	EquipItem(EquipmentDefinition);
}

void UWitchPTEquipmentManagerComponent::UnequipItem(UWitchPTEquipmentInstance* ItemInstance)
{
	if (GetOwner()->GetLocalRole() < ROLE_Authority)
	{
		Server_UnequipItem(ItemInstance);
	}
	else
	{
		if (ItemInstance != nullptr)
		{
			if (IsUsingRegisteredSubObjectList())
			{
				RemoveReplicatedSubObject(ItemInstance);
			}

			ItemInstance->OnUnequipped();
			OnItemUnequipped.Broadcast(ItemInstance);
			EquipmentList.RemoveEntry(ItemInstance);
		}
	}
}

void UWitchPTEquipmentManagerComponent::Server_UnequipItem_Implementation(UWitchPTEquipmentInstance* ItemInstance)
{
	UnequipItem(ItemInstance);
}


bool UWitchPTEquipmentManagerComponent::IsInventoryItemEquipped(UWitchPTInventoryItemInstance* InventoryItem) const
{
	return FindEquipmentByInventoryItem(InventoryItem) != nullptr;
}

UWitchPTEquipmentInstance* UWitchPTEquipmentManagerComponent::FindEquipmentByInventoryItem(
	UWitchPTInventoryItemInstance* InventoryItem) const
{
	for (const FWitchPTEquipmentEntry& Entry : EquipmentList.Entries)
	{
		if (UWitchPTEquipmentInstance* Equipment = Entry.Instance)
		{
			if (Equipment && Equipment->GetInstigator() == InventoryItem)
			{
				return Equipment;
			}
		}
	}
	return nullptr;
}

void UWitchPTEquipmentManagerComponent::PrintEquippedItems()
{
	UE_LOG(LogTemp, Warning, TEXT("📦 EQUIPPED ITEMS: %d total"), EquipmentList.Entries.Num());
	for (int32 i = 0; i < EquipmentList.Entries.Num(); i++)
	{
		const FWitchPTEquipmentEntry& Entry = EquipmentList.Entries[i];
		UE_LOG(LogTemp, Warning, TEXT("  [%d]: %s"), i, Entry.Instance ? *Entry.Instance->GetClass()->GetName() : TEXT("NULL"));
	}
}

UAbilitySystemComponent* UWitchPTEquipmentManagerComponent::GetAbilitySystemComponent() const
{
	if (APawn* OwnerPawn = Cast<APawn>(GetOwner()))
	{
		return UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwnerPawn);
	}
	return nullptr;
}



