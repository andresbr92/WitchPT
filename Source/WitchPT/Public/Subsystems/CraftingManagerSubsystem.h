// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CraftingManagerSubsystem.generated.h"

class UWitchPTInventoryItemInstance;
class UWitchPTInventoryItemDefinition;
/**
 * 
 */
USTRUCT()
struct FCalculatedPotionStats
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FText Name;
	UPROPERTY()
	FText Description;
	UPROPERTY()
	float FinalPotency;
	UPROPERTY()
	float FinalDuration;
	UPROPERTY()
	int32 FinalCharges;
	UPROPERTY()
	FGameplayTagContainer GrantedEffectTags;
	UPROPERTY()
	FGameplayTagContainer GrantedPropertyTags;
	UPROPERTY()
	bool bIsValidPotion;
};
USTRUCT(BlueprintType)
struct FPotionRecipeRow
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FText RecipeName;
	UPROPERTY()
	FText RecipeDescription;
	UPROPERTY()
	FGameplayTag BaseIngredientTag;
	UPROPERTY()
	FGameplayTag PrincipalIngredientTag;
	UPROPERTY()
	FGameplayTag ModifierIngredientTag;
	
};
UCLASS()
class WITCHPT_API UCraftingManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
public:
	UPROPERTY(EditAnywhere, Category="Config")
	TSoftObjectPtr<UDataTable> PotionRecipesTable;

	
	FCalculatedPotionStats CalculatePotionResult(UWitchPTInventoryItemInstance* BaseIngredient, UWitchPTInventoryItemInstance* PrincipalIngredient, UWitchPTInventoryItemInstance* ModifierIngredient) const;
	
	FCalculatedPotionStats CraftPotion(UWitchPTInventoryItemInstance* BaseIngredient, UWitchPTInventoryItemInstance* PrincipalIngredient, UWitchPTInventoryItemInstance* ModifierIngredient) const;
	
private:
	
	FPotionRecipeRow* FindRecipeByTags(const FGameplayTag& BaseTag, const FGameplayTag& PrincipalTag, const FGameplayTag& ModifierTag) const;
	
	
};
