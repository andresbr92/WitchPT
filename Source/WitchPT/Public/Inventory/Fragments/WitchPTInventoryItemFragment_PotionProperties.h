// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Inventory/Fragments/WitchPTInventoryItemFragment.h"
#include "WitchPTInventoryItemFragment_PotionProperties.generated.h"

/**
 * Fragment that defines potion properties for crafted potions
 * This stores the calculated properties from the cauldron crafting system
 */
UCLASS()
class WITCHPT_API UWitchPTInventoryItemFragment_PotionProperties : public UWitchPTInventoryItemFragment
{
	GENERATED_BODY()

public:
	// Core Potion Properties
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Potion Properties")
	FGameplayTag PotionEssenceTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Potion Properties")
	FGameplayTag BaseLiquidTypeTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Potion Properties")
	float FinalPotency = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Potion Properties")
	float FinalDuration = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Potion Properties")
	int32 FinalCharges = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Potion Properties")
	FGameplayTag PotionTierTag;

	// Effect and Property Tags
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Potion Effects")
	FGameplayTagContainer GrantedEffectTags;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Potion Effects")
	FGameplayTagContainer GrantedPropertyTags;

	// Helper functions
	UFUNCTION(BlueprintPure, Category = "Potion Properties")
	bool IsValidPotion() const;

	UFUNCTION(BlueprintPure, Category = "Potion Properties")
	FString GetPotionDescription() const;

	// Function to set properties from crafting system
	UFUNCTION(BlueprintCallable, Category = "Potion Properties")
	void SetPotionProperties(const FGameplayTag& InPotionEssence,
							const FGameplayTag& InBaseLiquidType,
							float InPotency,
							float InDuration,
							int32 InCharges,
							const FGameplayTag& InTierTag,
							const FGameplayTagContainer& InEffectTags,
							const FGameplayTagContainer& InPropertyTags);

	virtual void OnInstanceCreated(UWitchPTInventoryItemInstance* Instance) const override;
}; 