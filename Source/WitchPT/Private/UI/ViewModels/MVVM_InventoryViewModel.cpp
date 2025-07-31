// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModels/MVVM_InventoryViewModel.h"

#include "Equipment/WitchPTEquipmentManagerComponent.h"
#include "GameFramework/Character.h"
#include "Inventory/WitchPTInventoryItemInstance.h"
#include "Inventory/WitchPTInventoryManagerComponent.h"
#include "Inventory/Fragments/WitchPTInventoryItemFragment_EquippableItem.h"
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

void UMVVM_InventoryViewModel::EquipItem(UWitchPTInventoryItemInstance* ItemInstance)
{
	if (ItemInstance)
	{
		if (ACharacter* OwningCharacter = Cast<ACharacter>(PlayerController->GetPawn()))
		{
			if (UWitchPTEquipmentManagerComponent* EquipmentManager = OwningCharacter->FindComponentByClass<UWitchPTEquipmentManagerComponent>())
			{
				if (UWitchPTEquipmentInstance* EquipmentInstance = EquipmentManager->FindEquipmentByInventoryItem(ItemInstance))
				{
					EquipmentManager->UnequipItem(EquipmentInstance);
				}
				else
				{
					const UWitchPTInventoryItemFragment* BaseFragment = ItemInstance->FindFragmentByClass(UWitchPTInventoryItemFragment_EquippableItem::StaticClass());
					const UWitchPTInventoryItemFragment_EquippableItem* EquipableFragment = Cast<const UWitchPTInventoryItemFragment_EquippableItem>(BaseFragment);
					EquipmentManager->EquipItem(EquipableFragment->EquipmentDefinition);
				}
			}
		}
	}
}

void UMVVM_InventoryViewModel::RemoveItemStack(UWitchPTInventoryItemInstance* ItemInstance, int32 AmountToRemove)
{
	if (!ItemInstance)
	{
		return;
	}

	if (AWitchPTPlayerController* WitchPtPlayerController = Cast<AWitchPTPlayerController>(PlayerController))
	{
		if (UWitchPTInventoryManagerComponent* InventoryManager = WitchPtPlayerController->GetInventoryManager())
		{
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
	}
}
