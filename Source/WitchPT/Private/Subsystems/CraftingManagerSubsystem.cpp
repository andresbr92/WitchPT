// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/CraftingManagerSubsystem.h"

#include "Game/WitchPT_GameInstance.h"
#include "Inventory/WitchPTInventoryItemDefinition.h"
#include "Inventory/WitchPTInventoryItemInstance.h"
#include "Inventory/Fragments/InventoryFragment_IngredientCraftingProperties.h"
#include "Subsystems/CMS_Settings.h"

void UCraftingManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	UWitchPT_GameInstance* GI = Cast<UWitchPT_GameInstance>(GetGameInstance());

	if (UCMS_Settings::Get()->PotionRecipesTableRef.IsNull())
	{
		UE_LOG(LogTemp, Warning, TEXT("No PotionRecipesTableRef set in CMS_Settings!"));
		return;
	}
	if (!PotionRecipesTable)
	{
		if (UDataTable* LoadedTable = UCMS_Settings::Get()->PotionRecipesTableRef.LoadSynchronous())
		{
			PotionRecipesTable = LoadedTable;
			UE_LOG(LogTemp, Log, TEXT("CraftingManagerSubsystem initialized with PotionRecipesTable"));
		}
	}
}

FCalculatedPotionStats UCraftingManagerSubsystem::CalculatePotionResult(UWitchPTInventoryItemInstance* BaseIngredient,
	UWitchPTInventoryItemInstance* PrincipalIngredient, UWitchPTInventoryItemInstance* ModifierIngredient) const
{
	//First, validate that the recipe exist in the table for the given ingredients
	const UInventoryFragment_IngredientCraftingProperties* BaseCraftingProps = nullptr;
	const UInventoryFragment_IngredientCraftingProperties* PrincipalCraftingProps = nullptr;
	const UInventoryFragment_IngredientCraftingProperties* ModifierCraftingProps = nullptr;
	FCalculatedPotionStats ResultPotionStats;
	FGameplayTagContainer CombinedEffectTags;
	FGameplayTag BaseTag;
	FGameplayTag PrincipalTag;
	FGameplayTag ModifierTag;
	
	// Extract the fragment data from each ingredient
	if (BaseIngredient)
	{
		BaseCraftingProps = BaseIngredient->FindFragmentByClass<UInventoryFragment_IngredientCraftingProperties>();
		BaseTag = BaseCraftingProps->IngredientTypeTag;
		CombinedEffectTags.AppendTags(BaseCraftingProps->GrantedEffectTags);
	}
	if (PrincipalIngredient)
	{
		PrincipalCraftingProps = PrincipalIngredient->FindFragmentByClass<UInventoryFragment_IngredientCraftingProperties>();
		PrincipalTag = PrincipalCraftingProps->IngredientTypeTag;
		CombinedEffectTags.AppendTags(PrincipalCraftingProps->GrantedEffectTags);
	}
	if (ModifierIngredient)
	{
		
		ModifierCraftingProps = ModifierIngredient->FindFragmentByClass<UInventoryFragment_IngredientCraftingProperties>();
		ModifierTag = ModifierCraftingProps->IngredientTypeTag;
		CombinedEffectTags.AppendTags(ModifierCraftingProps->GrantedEffectTags);
	}
	// LOG the tags for debugging
	UE_LOG(LogTemp, Log, TEXT("BaseTag: %s, PrincipalTag: %s, ModifierTag: %s"), *BaseTag.ToString(), *PrincipalTag.ToString(), *ModifierTag.ToString());
	FPotionRecipeRow* FoundRecipe = FindRecipeByTags(BaseTag, PrincipalTag, ModifierTag);
	if (!FoundRecipe)
	{
		UE_LOG(LogTemp, Warning, TEXT("No valid recipe found for the given ingredients!"));
		ResultPotionStats.bIsValidPotion = false;
		
		return ResultPotionStats;
	}

	
	ResultPotionStats.GrantedEffectTags = CombinedEffectTags;
	ResultPotionStats.bIsValidPotion = true;
	ResultPotionStats.FinalCharges = 1;
	ResultPotionStats.FinalDuration = 30.f;
	ResultPotionStats.FinalPotency = 1.f;
	ResultPotionStats.Name = FoundRecipe->RecipeName;
	ResultPotionStats.Description = FoundRecipe->RecipeDescription;
	
	return ResultPotionStats;
	
}

FCalculatedPotionStats UCraftingManagerSubsystem::CraftPotion(UWitchPTInventoryItemInstance* BaseIngredient, UWitchPTInventoryItemInstance* PrincipalIngredient,
                                                                      UWitchPTInventoryItemInstance* ModifierIngredient) const
{
	if (!BaseIngredient || !PrincipalIngredient)
	{
		UE_LOG(LogTemp, Warning, TEXT("Base or Principal ingredient is null!"));
		return FCalculatedPotionStats();
	}
	FCalculatedPotionStats CalculatedStats = CalculatePotionResult(BaseIngredient, PrincipalIngredient, ModifierIngredient);

	
	return CalculatedStats;
	
}

FPotionRecipeRow* UCraftingManagerSubsystem::FindRecipeByTags(const FGameplayTag& BaseTag,
                                                              const FGameplayTag& PrincipalTag, const FGameplayTag& ModifierTag) const
{
	if (PotionRecipesTable.IsValid())
	{
		// Iterate through all rows in the data table
		for (const auto& RowPair : PotionRecipesTable.Get()->GetRowMap())
		{
			const FPotionRecipeRow* Recipe = reinterpret_cast<const FPotionRecipeRow*>(RowPair.Value);
			
			// Check if BaseTag and PrincipalTag match
			if (Recipe->BaseIngredientTag == BaseTag && Recipe->PrincipalIngredientTag == PrincipalTag)
			{
				// Handle optional ModifierTag
				// If ModifierTag parameter is valid, it must match the recipe's ModifierTag
				// If ModifierTag parameter is not valid, recipe's ModifierTag should also be invalid
				if (ModifierTag.IsValid())
				{
					if (Recipe->ModifierIngredientTag == ModifierTag)
					{
						return const_cast<FPotionRecipeRow*>(Recipe);
					}
				}
				else
				{
					// ModifierTag is not provided, so accept recipes without modifier or with invalid modifier
					if (!Recipe->ModifierIngredientTag.IsValid())
					{
						return const_cast<FPotionRecipeRow*>(Recipe);
					}
				}
			}
		}
	}
	
	return nullptr;
}
