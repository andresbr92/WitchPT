// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/WitchPTInventoryManagerComponent.h"

#include "Blueprint/UserWidget.h"
#include "Engine/ActorChannel.h"
#include "Inventory/WitchPTInventoryItemDefinition.h"
#include "Inventory/Fragments/WitchPTInventoryItemFragment.h"
#include "Inventory/WitchPTInventoryItemInstance.h"
#include "Inventory/Fragments/WitchPTInventoryFragment_Stackable.h"
#include "Inventory/Fragments/WitchPTInventoryItemFragment_IngredientDetails.h"
#include "Net/UnrealNetwork.h"
#include "UI/Widgets/Inventory/InventoryUserWidget.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(WitchPTInventoryManagerComponent)


void UWitchPTInventoryManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	ConstructInventory();
}

UWitchPTInventoryManagerComponent::UWitchPTInventoryManagerComponent(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
	, InventoryList(this)
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
	bReplicateUsingRegisteredSubObjectList = true;
}
void UWitchPTInventoryManagerComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, InventoryList);
}

// --------------- Inventory Manager Component Functions ---------------------------

void UWitchPTInventoryManagerComponent::Server_AddStacksToItem_Implementation(TSubclassOf<UWitchPTInventoryItemDefinition> ItemDef)
{
	if(IsValid(ItemDef))
	{
		UWitchPTInventoryItemInstance* FoundItem = FindFirstItemStackByDefinition(ItemDef);
		const UWitchPTInventoryItemFragment* BaseFragment = FoundItem->FindFragmentByClass(UWitchPTInventoryFragment_Stackable::StaticClass());
		const UWitchPTInventoryFragment_Stackable* StackableFragment = Cast<UWitchPTInventoryFragment_Stackable>(BaseFragment);
		
		int32 NewStackCount = FoundItem->GetTotalStackCount() + StackableFragment->GetStackCount();
		Server_UpdateItemStackCount(FoundItem, NewStackCount);
	}
}

void UWitchPTInventoryManagerComponent::Server_UpdateItemStackCount_Implementation(UWitchPTInventoryItemInstance* ItemInstance, int32 NewCount)
{
	if (IsValid(ItemInstance))
	{
		ItemInstance->SetTotalStackCount(NewCount);
		
		// Marcar la entrada como sucia para la replicación
		for (FWitchPTInventoryEntry& Entry : InventoryList.Entries)
		{
			if (Entry.Instance == ItemInstance)
			{
				InventoryList.MarkItemDirty(Entry);
				break;
			}
		}
		
		if (GetOwner()->GetNetMode() == NM_ListenServer || GetOwner()->GetNetMode() == NM_Standalone)
		{
			OnItemStackChanged.Broadcast(ItemInstance);
		}
	}
}

void UWitchPTInventoryManagerComponent::Server_RemoveItemInstance_Implementation(UWitchPTInventoryItemInstance* ItemInstance)
{
	if (IsValid(ItemInstance))
	{
		// Notificar antes de eliminar para que los listeners puedan usar la información del item
		if (GetOwner()->GetNetMode() == NM_ListenServer || GetOwner()->GetNetMode() == NM_Standalone)
		{
			OnItemRemoved.Broadcast(ItemInstance);
		}
		
		// Eliminar el item de la lista de inventario
		InventoryList.RemoveEntry(ItemInstance);
		
		// Desregistrar el subobjeto para la replicación
		if (IsUsingRegisteredSubObjectList())
		{
			RemoveReplicatedSubObject(ItemInstance);
		}
	}
}

void UWitchPTInventoryManagerComponent::Server_RemoveItemStacks_Implementation(UWitchPTInventoryItemInstance* ItemInstance, int32 AmountToRemove)
{
	if (IsValid(ItemInstance))
	{
		const int32 CurrentCount = ItemInstance->GetTotalStackCount();
		const int32 NewCount = FMath::Max(0, CurrentCount - AmountToRemove);
		
		// Si el nuevo conteo es cero, eliminar el item completamente
		if (NewCount <= 0)
		{
			Server_RemoveItemInstance(ItemInstance);
		}
		else
		{
			// Actualizar el conteo de stacks
			Server_UpdateItemStackCount(ItemInstance, NewCount);
		}
	}
}

TArray<UWitchPTInventoryItemInstance*> UWitchPTInventoryManagerComponent::GetAllItems() const
{
	return InventoryList.GetAllItems();
}

bool UWitchPTInventoryManagerComponent::TryAddItemDefinition(TSubclassOf<UWitchPTInventoryItemDefinition> ItemDef)
{
	if (!ItemDef)
	{
		return false;
	}
	
	UWitchPTInventoryItemInstance* FoundItem = FindFirstItemStackByDefinition(ItemDef);
	if (FoundItem)
	{
		Server_AddStacksToItem(ItemDef);
		return false;
	}
	else
	{
		Server_AddItem(ItemDef);
		return true;
	}
}



void UWitchPTInventoryManagerComponent::Server_AddItem_Implementation(TSubclassOf<UWitchPTInventoryItemDefinition> ItemDef)
{
	UWitchPTInventoryItemInstance* NewItem = nullptr;
	if (ItemDef != nullptr)
	{
		NewItem = InventoryList.AddEntry(ItemDef);
		if (GetOwner()->GetNetMode() == NM_ListenServer || GetOwner()->GetNetMode() == NM_Standalone)
		{
			OnItemAdded.Broadcast(NewItem);
		}
	
		if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && NewItem)
		{
			AddReplicatedSubObject(NewItem);
		}
	}
	
}



// void UWitchPTInventoryManagerComponent::RemoveItemInstance(UWitchPTInventoryItemInstance* ItemInstance)
// {
// 	InventoryList.RemoveEntry(ItemInstance);
//
// 	if (ItemInstance && IsUsingRegisteredSubObjectList())
// 	{
// 		RemoveReplicatedSubObject(ItemInstance);
// 	}
// }


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

void UWitchPTInventoryManagerComponent::ToggleInventoryMenu()
{
	if (bInventoryMenuOpen)
	{
		CloseInventoryMenu();
	}
	else
	{
		OpenInventoryMenu();
	}
}

void UWitchPTInventoryManagerComponent::ConstructInventory()
{
	OwningController = Cast<APlayerController>(GetOwner());
	checkf(OwningController.IsValid(), TEXT("Inventory Component should have a Player Controller as Owner."))
	if (!OwningController->IsLocalController()) return;
	InventoryMenu = CreateWidget<UInventoryUserWidget>(OwningController.Get(), InventoryMenuClass);
	InventoryMenu->AddToViewport();
	CloseInventoryMenu();
}

void UWitchPTInventoryManagerComponent::OpenInventoryMenu()
{
	if (!IsValid(InventoryMenu)) return;

	InventoryMenu->SetVisibility(ESlateVisibility::Visible);
	bInventoryMenuOpen = true;

	if (!OwningController.IsValid()) return;

	FInputModeGameAndUI InputMode;
	
	OwningController->SetInputMode(InputMode);
	OwningController->SetShowMouseCursor(true);
}

void UWitchPTInventoryManagerComponent::CloseInventoryMenu()
{
	if (!IsValid(InventoryMenu)) return;

	InventoryMenu->SetVisibility(ESlateVisibility::Collapsed);
	bInventoryMenuOpen = false;

	if (!OwningController.IsValid()) return;

	FInputModeGameOnly InputMode;
	OwningController->SetInputMode(InputMode);
	OwningController->SetShowMouseCursor(false);
}

