// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/WitchPTInventoryFunctionLibrary.h"

#include "Inventory/WitchPTInventoryItemDefinition.h"
#include "Inventory/Fragments/WitchPTInventoryItemFragment.h"

const UWitchPTInventoryItemFragment* UWitchPTInventoryFunctionLibrary::FindItemDefinitionFragment(
	TSubclassOf<UWitchPTInventoryItemDefinition> ItemDef, TSubclassOf<UWitchPTInventoryItemFragment> FragmentClass)
{
	if ((ItemDef != nullptr) && (FragmentClass != nullptr))
	{
		return GetDefault<UWitchPTInventoryItemDefinition>(ItemDef)->FindFragmentByClass(FragmentClass);
	}
	return nullptr;
}
