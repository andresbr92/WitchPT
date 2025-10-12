// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inv_InventoryWidgetController.h"

#include "AndresD3v_InventorySystem.h"
#include "InventoryManagement/Components/Inv_InventoryComponent.h"

void UInv_InventoryWidgetController::Activate()
{
}

void UInv_InventoryWidgetController::Deactivate()
{
}

void UInv_InventoryWidgetController::SetInventoryComponent(UInv_InventoryComponent* InInventoryComponent)
{
	if (InventoryComponent)
	{
		InventoryComponent->OnItemAdded.RemoveAll(this);
		InventoryComponent->OnItemRemoved.RemoveAll(this);
	}

	InventoryComponent = InInventoryComponent;


	if (InventoryComponent)
	{
		InventoryComponent->OnItemAdded.AddDynamic(this, &UInv_InventoryWidgetController::OnItemAdded);
		InventoryComponent->OnItemRemoved.AddDynamic(this, &UInv_InventoryWidgetController::OnItemRemoved);
		
		BroadcastInitialValues();
	}
}

void UInv_InventoryWidgetController::BroadcastInitialValues()
{
	if (!InventoryComponent) return;

	
	InventoryItems = InventoryComponent->GetAllItems();

	
	OnInventoryListChanged.Broadcast();
}

void UInv_InventoryWidgetController::BindCallbacksToDependencies()
{
	if (InventoryComponent)
	{
		UnbindCallbacksFromDependencies();
	}

	InventoryComponent->OnItemAdded.AddDynamic(this, &ThisClass::OnItemAdded);
	InventoryComponent->OnItemRemoved.AddDynamic(this, &ThisClass::OnItemRemoved);

	
}

void UInv_InventoryWidgetController::UnbindCallbacksFromDependencies()
{
	if (InventoryComponent)
	{
		InventoryComponent->OnItemAdded.RemoveAll(this);
		InventoryComponent->OnItemRemoved.RemoveAll(this);
	}
}

void UInv_InventoryWidgetController::OnItemAdded(UInv_InventoryItem* Item)
{
	UE_LOG(LogInventory, Warning, TEXT("OnItemAdded"));
	BroadcastInitialValues();
}

void UInv_InventoryWidgetController::OnItemRemoved(UInv_InventoryItem* Item)
{
	UE_LOG(LogInventory, Warning, TEXT("OnItemRemoved"));
	BroadcastInitialValues();
}
