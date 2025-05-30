// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Inventory/Fragments/WitchPTInventoryItemFragment.h"
#include "WitchPTInventoryItemFragment_IngredientCraftingProperties.generated.h"

USTRUCT(BlueprintType)
struct FBaseIngredientData
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag BaseLiquidTypeTag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 DefaultChargesProvided;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float StabilityModifier;
};

USTRUCT(BlueprintType)
struct FPrincipalIngredientData 
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag PrincipalEssenceTag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BasePotency;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BaseDuration;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bIsRitualEssence;
};

USTRUCT(BlueprintType)
struct FModifierIngredientData 
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTagContainer ModifierEffectTags;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float PotencyModificationValue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DurationModificationValue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag GrantedPropertyTag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AddedEffectTag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float StrengthOfModifier;
	
};
UCLASS()
class WITCHPT_API UWitchPTInventoryItemFragment_IngredientCraftingProperties : public UWitchPTInventoryItemFragment
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag SlotUsageTag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag TierTag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag CategoryTag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FBaseIngredientData BaseIngredientData;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FPrincipalIngredientData PrincipalIngredientData;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FModifierIngredientData ModifierIngredientData;

	virtual void OnInstanceCreated(UWitchPTInventoryItemInstance* Instance) const override;
};
