// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetControllers/QuickBarWidgetController.h"

#include "Character/WitchPTCharacter.h"
#include "Equipment/WitchPTEquipmentManagerComponent.h"
#include "Equipment/WitchPTQuickBarComponent.h"
#include "GameFramework/Character.h"
#include "Inventory/WitchPTInventoryItemInstance.h"
#include "Inventory/Fragments/WitchPTInventoryItemFragment_EquippableItem.h"
#include "Player/WitchPTPlayerController.h"
#include "Subsystems/WidgetCommunicatorSubsystem.h"

void UQuickBarWidgetController::SetContextualObject(UObject* ContextualObject)
{
	Super::SetContextualObject(ContextualObject);
}

void UQuickBarWidgetController::BroadcastInitialValues()
{

}

void UQuickBarWidgetController::BindCallbacksToDependencies()
{
	
	if (AWitchPTPlayerController* WitchPtPlayerController = Cast<AWitchPTPlayerController>(PlayerController))
	{
		
		if (UWitchPTQuickBarComponent* QuickBarComponent = WitchPtPlayerController->GetQuickBarComponent())
		{
			QuickBarComponent->OnActiveSlotChanged.AddDynamic(this, &UQuickBarWidgetController::OnActiveSlotChanged);
		}
	}

	// Bind UI changes from WidgetCommunicatorSubsystem
	
	if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
	{
		if (UWidgetCommunicatorSubsystem* WidgetCommunicator = LocalPlayer->GetSubsystem<UWidgetCommunicatorSubsystem>())
		{
			WidgetCommunicator->OnItemDraggedDelegate.AddDynamic(this, &UQuickBarWidgetController::OnEquipableItemDragStart);
		}
	}
	
}

void UQuickBarWidgetController::AddItemToQuickBarSlot(int32 SlotIndex, UWitchPTInventoryItemInstance* ItemInstance)
{
	if (ItemInstance != nullptr)
	{
		if (AWitchPTPlayerController* WitchPtPlayerController = Cast<AWitchPTPlayerController>(PlayerController))
		{
			WitchPtPlayerController->GetQuickBarComponent()->AddItemToSlot(SlotIndex, ItemInstance);
			
			
		}
	}
	
}

void UQuickBarWidgetController::OnActiveSlotChanged(int32 NewActiveSlotIndex)
{
	OnActiveSlotChangedDelegate.Broadcast(NewActiveSlotIndex);
}

void UQuickBarWidgetController::OnEquipableItemDragStart(const UWitchPTInventoryItemInstance* ItemInstance)
{
	if (ItemInstance != nullptr)
	{
		// Find the equipable fragment in the item instance
		const UWitchPTInventoryItemFragment_EquippableItem* EquipableFragment = 
			Cast<UWitchPTInventoryItemFragment_EquippableItem>(
				ItemInstance->FindFragmentByClass(UWitchPTInventoryItemFragment_EquippableItem::StaticClass()));
		if (EquipableFragment)
		{
			OnEquipableItemDragStartDelegate.Broadcast();
		}
		
		
	}
}
