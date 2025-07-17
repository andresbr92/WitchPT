// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetControllers/InventoryWidgetController.h"

#include "Equipment/WitchPTEquipmentManagerComponent.h"
#include "GameFramework/Character.h"
#include "Inventory/WitchPTInventoryItemInstance.h"
#include "Inventory/WitchPTInventoryManagerComponent.h"
#include "Inventory/Fragments/WitchPTInventoryItemFragment_EquippableItem.h"
#include "Player/WitchPTPlayerController.h"
#include "Subsystems/WidgetCommunicatorSubsystem.h"

class UWitchPTEquipmentInstance;
class UWitchPTEquipmentManagerComponent;

void UInventoryWidgetController::SetContextualObject(UObject* ContextualObject)
{
	Super::SetContextualObject(ContextualObject);
}

void UInventoryWidgetController::BindCallbacksToDependencies()
{
	AWitchPTPlayerController* WitchPtPlayerController = Cast<AWitchPTPlayerController>(PlayerController);
	if (WitchPtPlayerController)
	{
		UWitchPTInventoryManagerComponent* InventoryManager = WitchPtPlayerController->GetInventoryManager();
		if (InventoryManager)
		{
			InventoryManager->OnItemAdded.AddLambda([this](UWitchPTInventoryItemInstance* ItemAdded)
			{
				OnItemAddedDelegate.Broadcast(ItemAdded);
			});
			InventoryManager->OnItemStackChanged.AddLambda([this](UWitchPTInventoryItemInstance* ItemAdded)
			{
				OnItemStackChangedDelegate.Broadcast(ItemAdded);
			});
			InventoryManager->OnItemRemoved.AddLambda([this](UWitchPTInventoryItemInstance* ItemRemoved)
			{
				OnItemRemovedDelegate.Broadcast(ItemRemoved);
			});
		}
	}
}

void UInventoryWidgetController::RemoveItemStack(UWitchPTInventoryItemInstance* ItemInstance, int32 AmountToRemove)
{
	if (!ItemInstance)
	{
		return;
	}

	AWitchPTPlayerController* WitchPtPlayerController = Cast<AWitchPTPlayerController>(PlayerController);
	if (!WitchPtPlayerController)
	{
		return;
	}

	UWitchPTInventoryManagerComponent* InventoryManager = WitchPtPlayerController->GetInventoryManager();
	if (!InventoryManager)
	{
		return;
	}

	const int32 CurrentStackCount = ItemInstance->GetTotalStackCount();
	
	// If we only have 1 item left and trying to remove 1 or more, remove the item completely
	if (CurrentStackCount <= 1 && AmountToRemove >= 1)
	{
		InventoryManager->Server_RemoveItemInstance(ItemInstance);
	}
	else
	{
		// Otherwise, reduce the stack count by the requested amount
		InventoryManager->Server_RemoveItemStacks(ItemInstance, AmountToRemove);
	}
}

void UInventoryWidgetController::OnItemDragStart(UWitchPTInventoryItemInstance* ItemInstance)
{
	
	if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
	{
		if (UWidgetCommunicatorSubsystem* WidgetCommunicator = LocalPlayer->GetSubsystem<UWidgetCommunicatorSubsystem>())
		{
			WidgetCommunicator->OnItemDragged(ItemInstance);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("WidgetCommunicatorSubsystem not found in the world!"));
	}
}

void UInventoryWidgetController::EquipItem(UWitchPTInventoryItemInstance* ItemInstance)
{
	if (ItemInstance)
	{
		ACharacter* OwningCharacter = Cast<ACharacter>(PlayerController->GetPawn());
		if (OwningCharacter)
		{
			UWitchPTEquipmentManagerComponent* EquipmentManager = OwningCharacter->FindComponentByClass<UWitchPTEquipmentManagerComponent>();
			if (EquipmentManager)
			{
				UWitchPTEquipmentInstance* EquipmentInstance = EquipmentManager->FindEquipmentByInventoryItem(ItemInstance);
				if (EquipmentInstance)
				{
					EquipmentManager->UnequipItem(EquipmentInstance);
				}
				else
				{
					const UWitchPTInventoryItemFragment* BaseFragment = ItemInstance->FindFragmentByClass(UWitchPTInventoryItemFragment_EquippableItem::StaticClass());
					const UWitchPTInventoryItemFragment_EquippableItem* EquippableFragment = Cast<const UWitchPTInventoryItemFragment_EquippableItem>(BaseFragment);
					EquipmentManager->EquipItem(EquippableFragment->EquipmentDefinition);
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Owning character is not a valid ACharacter instance."));
		}
	}
}
