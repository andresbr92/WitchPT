// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/WitchPTInventoryManagerComponent.h"

#include "Engine/ActorChannel.h"
#include "Inventory/WitchPTInventoryItemDefinition.h"
#include "Inventory/WitchPTInventoryItemFragment.h"
#include "Inventory/WitchPTInventoryItemInstance.h"
#include "Inventory/Fragments/WitchPTInventoryFragment_Stats.h"
#include "Inventory/Fragments/WitchPTInventoryItemFragment_IngredientDetails.h"
#include "Net/UnrealNetwork.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(WitchPTInventoryManagerComponent)

class FLifetimeProperty;
struct FReplicationFlags;
//////////////////////////////////////////////////////////////////////
// FWitchPTInventoryEntry
FString FWitchPTInventoryEntry::GetDebugString() const
{
	TSubclassOf<UWitchPTInventoryItemDefinition> ItemDef;
	if (Instance != nullptr)
	{
		ItemDef = Instance->GetItemDef();
	}

	return FString::Printf(TEXT("%s ( x %s)"), *GetNameSafe(Instance),  *GetNameSafe(ItemDef));
}



//////////////////////////////////////////////////////////////////////
// FWitchPTInventoryList
void FWitchPTInventoryList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	for (int32 Index : RemovedIndices)
	{
		
	}
}

void FWitchPTInventoryList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (int32 Index : AddedIndices)
	{
		
	}
}

void FWitchPTInventoryList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
	for (int32 Index : ChangedIndices)
	{
		
	}
}

void FWitchPTInventoryList::BroadcastChangeMessage(FWitchPTInventoryEntry& Entry, int32 OldCount, int32 NewCount)
{
	
}
UWitchPTInventoryItemInstance* FWitchPTInventoryList::AddEntry(TSubclassOf<UWitchPTInventoryItemDefinition> ItemDef, int32 StackCount)
{
	UWitchPTInventoryItemInstance* Result = nullptr;

	check(ItemDef != nullptr);
	check(OwnerComponent);

	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());


	FWitchPTInventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Instance = NewObject<UWitchPTInventoryItemInstance>(OwnerComponent->GetOwner());  //@TODO: Using the actor instead of component as the outer due to UE-127172
	NewEntry.Instance->SetItemDef(ItemDef);
	NewEntry.Instance->SetTotalStackCount(StackCount);
	for (UWitchPTInventoryItemFragment* Fragment : GetDefault<UWitchPTInventoryItemDefinition>(ItemDef)->Fragments)
	{
		if (Fragment != nullptr)
		{
			Fragment->OnInstanceCreated(NewEntry.Instance);
		}
	}

	Result = NewEntry.Instance;
	
	
	MarkItemDirty(NewEntry);

	return Result;
}

void FWitchPTInventoryList::AddEntry(UWitchPTInventoryItemInstance* Instance)
{
	unimplemented();
}

void FWitchPTInventoryList::RemoveEntry(UWitchPTInventoryItemInstance* Instance)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FWitchPTInventoryEntry& Entry = *EntryIt;
		if (Entry.Instance == Instance)
		{
			EntryIt.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}
TArray<UWitchPTInventoryItemInstance*> FWitchPTInventoryList::GetAllItems() const
{
	TArray<UWitchPTInventoryItemInstance*> Results;
	Results.Reserve(Entries.Num());
	for (const FWitchPTInventoryEntry& Entry : Entries)
	{
		if (Entry.Instance != nullptr) //@TODO: Would prefer to not deal with this here and hide it further?
		{
			Results.Add(Entry.Instance);
		}
	}
	return Results;
}


UWitchPTInventoryManagerComponent::UWitchPTInventoryManagerComponent(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
	, InventoryList(this)
{
	SetIsReplicatedByDefault(true);
}
void UWitchPTInventoryManagerComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, InventoryList);
}

// --------------- Inventory Manager Component Functions ---------------------------

TArray<UWitchPTInventoryItemInstance*> UWitchPTInventoryManagerComponent::GetAllItems() const
{
	return InventoryList.GetAllItems();
}

bool UWitchPTInventoryManagerComponent::CanAddItemDefinition(TSubclassOf<UWitchPTInventoryItemDefinition> ItemDef,
	int32 StackCount)
{
	if (!ItemDef)
	{
		return false;
	}
	
	UWitchPTInventoryItemInstance* FoundItem = FindFirstItemStackByDefinition(ItemDef);
	if (FoundItem)
	{
		FoundItem->SetTotalStackCount(FoundItem->GetTotalStackCount() + StackCount);
		if (GetOwner()->GetNetMode() == NM_ListenServer || GetOwner()->GetNetMode() == NM_Standalone)
		{
			OnItemStackChanged.Broadcast(FoundItem);
		}
		
	}
	else
	{
		AddItemDefinition(ItemDef, StackCount);
		return true;
	}
	return false;
}

UWitchPTInventoryItemInstance* UWitchPTInventoryManagerComponent::AddItemDefinition(
	TSubclassOf<UWitchPTInventoryItemDefinition> ItemDef, int32 StackCount)
{
	UWitchPTInventoryItemInstance* Result = nullptr;
	if (ItemDef != nullptr)
	{
		Result = InventoryList.AddEntry(ItemDef, StackCount);
		if (GetOwner()->GetNetMode() == NM_ListenServer || GetOwner()->GetNetMode() == NM_Standalone)
		{
			OnItemAdded.Broadcast(Result);
		}
		
		if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && Result)
		{
			AddReplicatedSubObject(Result);
		}
	}
	return Result;
}

void UWitchPTInventoryManagerComponent::AddItemInstance(UWitchPTInventoryItemInstance* ItemInstance)
{
	InventoryList.AddEntry(ItemInstance);
	if (GetOwner()->GetNetMode() == NM_ListenServer || GetOwner()->GetNetMode() == NM_Standalone)
	{
		OnItemAdded.Broadcast(ItemInstance);
	}
	if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && ItemInstance)
	{
		AddReplicatedSubObject(ItemInstance);
	}
}

void UWitchPTInventoryManagerComponent::RemoveItemInstance(UWitchPTInventoryItemInstance* ItemInstance)
{
	InventoryList.RemoveEntry(ItemInstance);

	if (ItemInstance && IsUsingRegisteredSubObjectList())
	{
		RemoveReplicatedSubObject(ItemInstance);
	}
}


UWitchPTInventoryItemInstance* UWitchPTInventoryManagerComponent::FindFirstItemStackByDefinition(
	TSubclassOf<UWitchPTInventoryItemDefinition> ItemDef) const
{
	for (const FWitchPTInventoryEntry& Entry : InventoryList.Entries)
	{
		UWitchPTInventoryItemInstance* Instance = Entry.Instance;

		if (IsValid(Instance))
		{
			if (Instance->GetItemDef() == ItemDef)
			{
				return Instance;
			}
		}
	}

	return nullptr;
}

int32 UWitchPTInventoryManagerComponent::GetTotalItemCountByDefinition(
	TSubclassOf<UWitchPTInventoryItemDefinition> ItemDef) const
{
	int32 TotalCount = 0;
	for (const FWitchPTInventoryEntry& Entry : InventoryList.Entries)
	{
		UWitchPTInventoryItemInstance* Instance = Entry.Instance;

		if (IsValid(Instance))
		{
			if (Instance->GetItemDef() == ItemDef)
			{
				++TotalCount;
			}
		}
	}

	return TotalCount;
}

bool UWitchPTInventoryManagerComponent::ConsumeItemsByDefinition(TSubclassOf<UWitchPTInventoryItemDefinition> ItemDef,
	int32 NumToConsume)
{
	AActor* OwningActor = GetOwner();
	if (!OwningActor || !OwningActor->HasAuthority())
	{
		return false;
	}

	//@TODO: N squared right now as there's no acceleration structure
	int32 TotalConsumed = 0;
	while (TotalConsumed < NumToConsume)
	{
		if (UWitchPTInventoryItemInstance* Instance = UWitchPTInventoryManagerComponent::FindFirstItemStackByDefinition(ItemDef))
		{
			InventoryList.RemoveEntry(Instance);
			++TotalConsumed;
		}
		else
		{
			return false;
		}
	}

	return TotalConsumed == NumToConsume;
}

bool UWitchPTInventoryManagerComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch,
	FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (FWitchPTInventoryEntry& Entry : InventoryList.Entries)
	{
		UWitchPTInventoryItemInstance* Instance = Entry.Instance;

		if (Instance && IsValid(Instance))
		{
			WroteSomething |= Channel->ReplicateSubobject(Instance, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}

void UWitchPTInventoryManagerComponent::ReadyForReplication()
{
	Super::ReadyForReplication();

	// Register existing ULyraInventoryItemInstance
	UE_LOG(LogTemp, Warning, TEXT("The boolean value is %s"), (IsUsingRegisteredSubObjectList() ? TEXT("true") : TEXT("false") ));
	
	UE_LOG(LogTemp, Warning, TEXT("The Actor's name is %s"), *UWitchPTInventoryManagerComponent::GetOwner()->GetName() );
	if (IsUsingRegisteredSubObjectList())
	{
		for (const FWitchPTInventoryEntry& Entry : InventoryList.Entries)
		{
			UWitchPTInventoryItemInstance* Instance = Entry.Instance;

			if (IsValid(Instance))
			{
				AddReplicatedSubObject(Instance);
			}
		}
	}
}

