// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Inventory/WitchPTInventoryItemFragment.h"
#include "WitchPTInventoryItemFragment_IngredientDetails.generated.h"

/**
 * 
 */
UCLASS()
class WITCHPT_API UWitchPTInventoryItemFragment_IngredientDetails : public UWitchPTInventoryItemFragment
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag IngredientTier;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag IngredientCategory;

	virtual void OnInstanceCreated(UWitchPTInventoryItemInstance* Instance) const override;
};
