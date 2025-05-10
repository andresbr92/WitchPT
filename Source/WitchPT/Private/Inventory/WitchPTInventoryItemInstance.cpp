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

	DOREPLIFETIME(ThisClass, StatTags);
	DOREPLIFETIME(ThisClass, ItemDef);
}

void UWitchPTInventoryItemInstance::AddStatTagStack(FGameplayTag Tag, int32 StackCount)
{
}

void UWitchPTInventoryItemInstance::RemoveStatTagStack(FGameplayTag Tag, int32 StackCount)
{
}

int32 UWitchPTInventoryItemInstance::GetStatTagStackCount(FGameplayTag Tag) const
{
	return 1;
}

bool UWitchPTInventoryItemInstance::HasStatTag(FGameplayTag Tag) const
{
	return true;
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

void UWitchPTInventoryItemInstance::SetItemDef(TSubclassOf<UWitchPTInventoryItemDefinition> InDef)
{
}
