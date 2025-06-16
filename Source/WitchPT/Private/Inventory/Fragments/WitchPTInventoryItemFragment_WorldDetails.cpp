// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory/Fragments/WitchPTInventoryItemFragment_WorldDetails.h"
#include "Inventory/WitchPTInventoryItemInstance.h"

void UWitchPTInventoryItemFragment_WorldDetails::OnInstanceCreated(UWitchPTInventoryItemInstance* Instance) const
{
	// Basic setup when instance is created
	// This could be expanded later for specific world item initialization
}

bool UWitchPTInventoryItemFragment_WorldDetails::HasValidWorldMesh() const
{
	return WorldStaticMesh != nullptr;
} 