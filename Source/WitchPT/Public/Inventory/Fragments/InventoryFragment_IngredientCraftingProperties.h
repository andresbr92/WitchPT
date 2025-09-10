// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Inventory/Fragments/WitchPTInventoryItemFragment.h"
#include "InventoryFragment_IngredientCraftingProperties.generated.h"


UCLASS()
class WITCHPT_API UInventoryFragment_IngredientCraftingProperties : public UWitchPTInventoryItemFragment
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag IngredientTypeTag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag SlotUsageTag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag TierTag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag CategoryTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crafting Effects")
	FGameplayTagContainer GrantedEffectTags;


	virtual void OnInstanceCreated(UWitchPTInventoryItemInstance* Instance) const override;
	
	
};
