// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory/Fragments/InventoryFragment_WorldDetails.h"
#include "Inventory/WitchPTInventoryItemInstance.h"

void UInventoryFragment_WorldDetails::OnInstanceCreated(UWitchPTInventoryItemInstance* Instance) const
{
	// Basic setup when instance is created
	// This could be expanded later for specific world item initialization
}

bool UInventoryFragment_WorldDetails::HasValidWorldMesh() const
{
	return WorldStaticMesh != nullptr;
} 