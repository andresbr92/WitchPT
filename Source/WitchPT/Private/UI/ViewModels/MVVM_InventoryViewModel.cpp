// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModels/MVVM_InventoryViewModel.h"
#include "Inventory/WitchPTInventoryManagerComponent.h"
#include "Player/WitchPTPlayerController.h"

void UMVVM_InventoryViewModel::BindCallbacksToDependencies()
{
	if (AWitchPTPlayerController* WitchPtPlayerController = Cast<AWitchPTPlayerController>(PlayerController))
	{
		if (UWitchPTInventoryManagerComponent* InventoryManager = WitchPtPlayerController->GetInventoryManager())
		{
			InventoryManager->OnItemAdded.AddLambda([this](UWitchPTInventoryItemInstance* ItemAdded)
			{
				AddItem(ItemAdded);
			});
			InventoryManager->OnItemStackChanged.AddLambda([this](UWitchPTInventoryItemInstance* ItemAdded)
			{
				
			});
			InventoryManager->OnItemRemoved.AddLambda([this](UWitchPTInventoryItemInstance* ItemRemoved)
			{
				RemoveItem(ItemRemoved);
			});
			
		}
	}
}

void UMVVM_InventoryViewModel::BroadcastInitialValues()
{
	if (AWitchPTPlayerController* WitchPtPlayerController = Cast<AWitchPTPlayerController>(PlayerController))
	{
		if (UWitchPTInventoryManagerComponent* InventoryManager = WitchPtPlayerController->GetInventoryManager())
		{
			SetItems(InventoryManager->GetAllItems());
		}
	}
	
}

void UMVVM_InventoryViewModel::AddItem(UWitchPTInventoryItemInstance* ItemInstance)
{
	if (ItemInstance)
	{
		Items.Add(ItemInstance);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(Items);
	}
}

void UMVVM_InventoryViewModel::RemoveItem(UWitchPTInventoryItemInstance* ItemInstance)
{
	if (ItemInstance)
	{
		Items.Remove(ItemInstance);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(Items);
	}
}

TArray<UWitchPTInventoryItemInstance*> UMVVM_InventoryViewModel::GetItems() const
{
	return Items;
}
