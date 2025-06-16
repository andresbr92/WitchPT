// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory/Fragments/WitchPTInventoryItemFragment_PotionProperties.h"
#include "Inventory/WitchPTInventoryItemInstance.h"

void UWitchPTInventoryItemFragment_PotionProperties::OnInstanceCreated(UWitchPTInventoryItemInstance* Instance) const
{
	// Basic setup when potion instance is created
	// This could be expanded later for specific potion initialization
}

bool UWitchPTInventoryItemFragment_PotionProperties::IsValidPotion() const
{
	// Basic validation: check if we have essential potion properties
	return PotionEssenceTag.IsValid() && 
		   FinalPotency > 0.0f && 
		   FinalDuration > 0.0f && 
		   FinalCharges > 0;
}

FString UWitchPTInventoryItemFragment_PotionProperties::GetPotionDescription() const
{
	if (!IsValidPotion())
	{
		return TEXT("Invalid Potion");
	}

	// Create a simple description string
	FString Description = FString::Printf(
		TEXT("%s Potion\nPotency: %.1f\nDuration: %.1fs\nCharges: %d"),
		*PotionEssenceTag.GetTagName().ToString(),
		FinalPotency,
		FinalDuration,
		FinalCharges
	);

	// Add tier information if available
	if (PotionTierTag.IsValid())
	{
		Description += FString::Printf(TEXT("\nTier: %s"), *PotionTierTag.GetTagName().ToString());
	}

	// Add base liquid information if available
	if (BaseLiquidTypeTag.IsValid())
	{
		Description += FString::Printf(TEXT("\nBase: %s"), *BaseLiquidTypeTag.GetTagName().ToString());
	}

	// Add effects count if present
	if (GrantedEffectTags.Num() > 0)
	{
		Description += FString::Printf(TEXT("\nEffects: %d"), GrantedEffectTags.Num());
	}

	// Add properties count if present
	if (GrantedPropertyTags.Num() > 0)
	{
		Description += FString::Printf(TEXT("\nProperties: %d"), GrantedPropertyTags.Num());
	}

	return Description;
}

void UWitchPTInventoryItemFragment_PotionProperties::SetPotionProperties(
	const FGameplayTag& InPotionEssence,
	const FGameplayTag& InBaseLiquidType,
	float InPotency,
	float InDuration,
	int32 InCharges,
	const FGameplayTag& InTierTag,
	const FGameplayTagContainer& InEffectTags,
	const FGameplayTagContainer& InPropertyTags)
{
	PotionEssenceTag = InPotionEssence;
	BaseLiquidTypeTag = InBaseLiquidType;
	FinalPotency = InPotency;
	FinalDuration = InDuration;
	FinalCharges = InCharges;
	PotionTierTag = InTierTag;
	GrantedEffectTags = InEffectTags;
	GrantedPropertyTags = InPropertyTags;

	UE_LOG(LogTemp, Log, TEXT("UWitchPTInventoryItemFragment_PotionProperties::SetPotionProperties: Potion properties set - %s, Potency: %.2f, Duration: %.2f, Charges: %d"), 
		   *PotionEssenceTag.ToString(), 
		   FinalPotency, 
		   FinalDuration, 
		   FinalCharges);
} 