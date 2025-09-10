// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/WitchPTInventoryItemInstance.h"

#include "GameplayTagContainer.h"
#include "Inventory/WitchPTInventoryItemDefinition.h"
#include "Inventory/Fragments/WitchPTInventoryItemFragment.h"
#include "Net/UnrealNetwork.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(WitchPTInventoryItemInstance)
class FLifetimeProperty;
UWitchPTInventoryItemInstance::UWitchPTInventoryItemInstance(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UWitchPTInventoryItemInstance::AddStatTagStack(FGameplayTag Tag, int32 StackCount)
{
	StatTags.AddStack(Tag, StackCount);
}

void UWitchPTInventoryItemInstance::RemoveStatTagStack(FGameplayTag Tag, int32 StackCount)
{
	StatTags.RemoveStack(Tag, StackCount);
}

int32 UWitchPTInventoryItemInstance::GetStatTagStackCount(FGameplayTag Tag) const
{
	return StatTags.GetStackCount(Tag);
}

bool UWitchPTInventoryItemInstance::HasStatTag(FGameplayTag Tag) const
{
	return StatTags.ContainsTag(Tag);
}

void UWitchPTInventoryItemInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, ItemDef);
	DOREPLIFETIME(ThisClass, TotalStackCount);
	DOREPLIFETIME(ThisClass, StatTags);
}



void UWitchPTInventoryItemInstance::SetItemManifest(TSubclassOf<UWitchPTInventoryItemDefinition> InDef)
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

