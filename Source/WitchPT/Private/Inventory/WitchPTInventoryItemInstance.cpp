// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/WitchPTInventoryItemInstance.h"

#include "GameplayTagContainer.h"
#include "Inventory/WitchPTInventoryItemDefinition.h"
#include "Inventory/WitchPTInventoryItemFragment.h"
#include "Net/UnrealNetwork.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(WitchPTInventoryItemInstance)
class FLifetimeProperty;
UWitchPTInventoryItemInstance::UWitchPTInventoryItemInstance(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UWitchPTInventoryItemInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, ItemDef);
	DOREPLIFETIME(ThisClass, TotalStackCount);
}



void UWitchPTInventoryItemInstance::SetItemDef(TSubclassOf<UWitchPTInventoryItemDefinition> InDef)
{
	ItemDef = InDef;
}

const UWitchPTInventoryItemFragment* UWitchPTInventoryItemInstance::FindFragmentByClass(
	TSubclassOf<UWitchPTInventoryItemFragment> FragmentClass) const
{
	if ((ItemDef != nullptr) && (FragmentClass != nullptr))
	{
		return GetDefault<UWitchPTInventoryItemDefinition>(ItemDef)->FindFragmentByClass(FragmentClass);
	}

	return nullptr;
}

