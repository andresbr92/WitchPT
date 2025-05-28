// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetControllers/InventoryWidgetController.h"

#include "Inventory/WitchPTInventoryItemInstance.h"
#include "Inventory/WitchPTInventoryManagerComponent.h"
#include "Player/WitchPTPlayerController.h"

void UInventoryWidgetController::BindCallbacksToDependencies()
{
	AWitchPTPlayerController* WitchPtPlayerController = Cast<AWitchPTPlayerController>(PlayerController);
	if (WitchPtPlayerController)
	{
		UWitchPTInventoryManagerComponent* InventoryManager = WitchPtPlayerController->GetInventoryManager();
		if (InventoryManager)
		{
			InventoryManager->OnItemAdded.AddLambda([this](UInventoryItemDefinition* ItemAdded)
			{
				OnItemAddedDelegate.Broadcast(ItemAdded);
			});
			InventoryManager->OnItemStackChanged.AddLambda([this](UInventoryItemDefinition* ItemAdded)
			{
				OnItemStackChangedDelegate.Broadcast(ItemAdded);
			});
			InventoryManager->OnItemRemoved.AddLambda([this](UInventoryItemDefinition* ItemRemoved)
			{
				OnItemRemovedDelegate.Broadcast(ItemRemoved);
			});
		}
	}
}

void UInventoryWidgetController::RemoveItemStack(UInventoryItemDefinition* ItemInstance, int32 AmountToRemove)
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
