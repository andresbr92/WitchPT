// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetControllers/InventoryWidgetController.h"

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
			InventoryManager->OnItemAdded.AddLambda([this](UWitchPTInventoryItemInstance* ItemAdded)
			{
				OnItemAddedDelegate.Broadcast(ItemAdded);
			});
			InventoryManager->OnItemStackChanged.AddLambda([this](UWitchPTInventoryItemInstance* ItemAdded)
			{
				OnItemStackChangedDelegate.Broadcast(ItemAdded);
			});
		}
	}
}
