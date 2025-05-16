// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/WitchPTInventoryItemDefinition.h"

#include "Inventory/Fragments/WitchPTInventoryItemFragment.h"

UWitchPTInventoryItemDefinition::UWitchPTInventoryItemDefinition(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)

{
}

const UWitchPTInventoryItemFragment* UWitchPTInventoryItemDefinition::FindFragmentByClass(
	TSubclassOf<UWitchPTInventoryItemFragment> FragmentClass) const
{
	if (FragmentClass != nullptr)
	{
		for (UWitchPTInventoryItemFragment* Fragment : Fragments)
		{
			if (Fragment && Fragment->IsA(FragmentClass))
			{
				return Fragment;
			}
		}
	}

	return nullptr;
}
