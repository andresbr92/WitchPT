// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Fragments/WitchPTInventoryFragment_Stats.h"

#include "Inventory/WitchPTInventoryItemInstance.h"

void UWitchPTInventoryFragment_Stats::OnInstanceCreated(UWitchPTInventoryItemInstance* Instance) const
{
	// for (const auto& KVP : InitialItemStats)
	// {
	// 	Instance->AddStatTagStack(KVP.Key, KVP.Value);
	// }
}

int32 UWitchPTInventoryFragment_Stats::GetItemStatByTag(FGameplayTag Tag) const
{
	if (const int32* StatPtr = InitialItemStats.Find(Tag))
	{
		return *StatPtr;
	}

	return 0;
}
