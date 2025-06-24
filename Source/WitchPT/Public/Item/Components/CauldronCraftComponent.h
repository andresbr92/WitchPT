// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameplayTagContainer.h"
#include "CauldronCraftComponent.generated.h"

class UWitchPTInventoryItemDefinition;
class UWitchPTInventoryItemInstance;
class ACharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBaseIngredientSetSignature, UWitchPTInventoryItemInstance*, IngredientInstance);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBaseIngredientIconSetSignature, TSubclassOf<UUserWidget>, BaseIngredientIcon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPrincipalIngredientSetSignature, UWitchPTInventoryItemInstance*, IngredientInstance);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPrincipalIngredientIconSetSignature, TSubclassOf<UUserWidget>, PrincipalIngredientIcon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnModifierIngredientSetSignature, UWitchPTInventoryItemInstance*, IngredientInstance);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnModifierIngredientIconSetSignature, TSubclassOf<UUserWidget>, ModifierIngredientIcon);

// Core Potion Crafting Structures
USTRUCT(BlueprintType)
struct FPotionResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag PotionEssenceTag;

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag BaseLiquidTypeTag;

	UPROPERTY(BlueprintReadOnly)
	float FinalPotency = 0.0f;

	UPROPERTY(BlueprintReadOnly)
	float FinalDuration = 0.0f;

	UPROPERTY(BlueprintReadOnly)
	int32 FinalCharges = 0;

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag PotionTierTag;

	UPROPERTY(BlueprintReadOnly)
	FGameplayTagContainer GrantedEffectTags;

	UPROPERTY(BlueprintReadOnly)
	FGameplayTagContainer GrantedPropertyTags;

	UPROPERTY(BlueprintReadOnly)
	bool bIsValid = false;

	UPROPERTY(BlueprintReadOnly)
	TSubclassOf<UWitchPTInventoryItemDefinition> ResultPotionItemDef;
};

USTRUCT(BlueprintType)
struct FPotionCraftingRecipe
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	bool bRequiresBaseIngredient = true;

	UPROPERTY(BlueprintReadOnly)
	bool bRequiresPrincipalIngredient = true;

	UPROPERTY(BlueprintReadOnly)
	bool bRequiresModifierIngredient = false;

	UPROPERTY(BlueprintReadOnly)
	FGameplayTagContainer AllowedBaseLiquidTypes;

	UPROPERTY(BlueprintReadOnly)
	FGameplayTagContainer AllowedPrincipalEssences;

	UPROPERTY(BlueprintReadOnly)
	FGameplayTagContainer IncompatibleCombinations;
};

// Extracted Ingredient Data Structures
USTRUCT(BlueprintType)
struct FExtractedBaseIngredientData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag BaseLiquidTypeTag;

	UPROPERTY(BlueprintReadOnly)
	int32 DefaultChargesProvided = 0;

	UPROPERTY(BlueprintReadOnly)
	float StabilityModifier = 1.0f;

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag TierTag;

	UPROPERTY(BlueprintReadOnly)
	bool bIsValid = false;
};

USTRUCT(BlueprintType)
struct FExtractedPrincipalIngredientData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag PrincipalEssenceTag;

	UPROPERTY(BlueprintReadOnly)
	float BasePotency = 0.0f;

	UPROPERTY(BlueprintReadOnly)
	float BaseDuration = 0.0f;

	UPROPERTY(BlueprintReadOnly)
	bool bIsRitualEssence = false;

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag TierTag;

	UPROPERTY(BlueprintReadOnly)
	bool bIsValid = false;
};

USTRUCT(BlueprintType)
struct FExtractedModifierIngredientData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FGameplayTagContainer ModifierEffectTags;

	UPROPERTY(BlueprintReadOnly)
	float PotencyModificationValue = 1.0f;

	UPROPERTY(BlueprintReadOnly)
	float DurationModificationValue = 0.0f;

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag GrantedPropertyTag;

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag AddedEffectTag;

	UPROPERTY(BlueprintReadOnly)
	float StrengthOfModifier = 1.0f;

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag TierTag;

	UPROPERTY(BlueprintReadOnly)
	bool bIsValid = false;
};

UCLASS(Blueprintable)
class WITCHPT_API UCauldronCraftComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCauldronCraftComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	// Replication
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	// Ingredient Variables
	UPROPERTY(VisibleAnywhere)
	UWitchPTInventoryItemInstance* BaseIngredient;
	UPROPERTY(VisibleAnywhere)
	UWitchPTInventoryItemInstance* PrincipalIngredient;
	UPROPERTY(VisibleAnywhere)
	UWitchPTInventoryItemInstance* ModifierIngredient;
	
	UPROPERTY(ReplicatedUsing = OnRep_BaseIngredientIcon, Category= "Cauldron|Ingredients", VisibleAnywhere)
	TSubclassOf<UUserWidget> BaseIngredientIcon;

	UPROPERTY(ReplicatedUsing = OnRep_PrincipalIngredientIcon, Category= "Cauldron|Ingredients", VisibleAnywhere)
	TSubclassOf<UUserWidget> PrincipalIngredientIcon;

	UPROPERTY(ReplicatedUsing = OnRep_PotentiatorIngredientIcon, Category= "Cauldron|Ingredients", VisibleAnywhere)
	TSubclassOf<UUserWidget> ModifierIngredientIcon;

	// Potion Crafting Variables
	UPROPERTY(ReplicatedUsing = OnRep_CraftedPotion, BlueprintReadOnly, Category = "Potion Crafting")
	FPotionResult CraftedPotion;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Potion Crafting")
	FPotionCraftingRecipe DefaultCraftingRecipe;

	// Delegates
	FOnBaseIngredientSetSignature OnBaseIngredientSetDelegate;
	FOnBaseIngredientIconSetSignature OnBaseIngredientIconSetDelegate;
	FOnPrincipalIngredientSetSignature OnPrincipalIngredientSetDelegate;
	FOnPrincipalIngredientIconSetSignature OnPrincipalIngredientIconSetDelegate;
	FOnModifierIngredientSetSignature OnModifierIngredientSetDelegate;
	FOnModifierIngredientIconSetSignature OnModifierIngredientIconSetDelegate;

	// Ingredient Functions
	void TrySetIngredientInSlot(const ACharacter* RequestingCharacter, const TSubclassOf<UWitchPTInventoryItemDefinition>& IngredientItemDef);
	
	// OnRep Functions
	UFUNCTION()
	void OnRep_BaseIngredientIcon();
	UFUNCTION()
	void OnRep_PrincipalIngredientIcon();
	UFUNCTION()
	void OnRep_PotentiatorIngredientIcon();
	UFUNCTION()
	void OnRep_CraftedPotion();

	// Crafting Validation Functions
	UFUNCTION(BlueprintPure, Category = "Potion Crafting")
	bool CanCraftPotion() const;

	UFUNCTION(BlueprintPure, Category = "Potion Crafting")
	FPotionCraftingRecipe GetRequiredIngredients() const;

	UFUNCTION(BlueprintPure, Category = "Potion Crafting")
	bool ValidateIngredientCombination() const;

	// Debug Functions
	UFUNCTION(BlueprintCallable, Category = "Debug|Potion Crafting")
	void PrintIngredientDebugData() const;

	UFUNCTION(BlueprintCallable, Category = "Debug|Potion Crafting")
	void PrintSpecificIngredientData(UWitchPTInventoryItemInstance* Ingredient, const FString& SlotName) const;

	// Ingredient Data Extraction Functions
	UFUNCTION(BlueprintPure, Category = "Potion Crafting|Data Extraction")
	FExtractedBaseIngredientData ExtractBaseIngredientProperties() const;

	UFUNCTION(BlueprintPure, Category = "Potion Crafting|Data Extraction")
	FExtractedPrincipalIngredientData ExtractPrincipalIngredientProperties() const;

	UFUNCTION(BlueprintPure, Category = "Potion Crafting|Data Extraction")
	FExtractedModifierIngredientData ExtractModifierIngredientProperties() const;

	// Potion Calculation Functions
	UFUNCTION(BlueprintCallable, Category = "Potion Crafting|Calculation")
	FPotionResult CalculatePotionProperties() const;

	UFUNCTION(BlueprintPure, Category = "Potion Crafting|Calculation")
	FGameplayTag CalculatePotionTier(const FExtractedBaseIngredientData& BaseData, 
									 const FExtractedPrincipalIngredientData& PrincipalData, 
									 const FExtractedModifierIngredientData& ModifierData) const;

	// Potion ItemDefinition Generation Functions
	UFUNCTION(BlueprintCallable, Category = "Potion Crafting|ItemDefinition")
	TSubclassOf<UWitchPTInventoryItemDefinition> GeneratePotionItemDefinition(const FPotionResult& PotionData) const;

	UFUNCTION(BlueprintCallable, Category = "Potion Crafting|ItemDefinition")
	UWitchPTInventoryItemDefinition* CreatePotionItemDefinitionInstance(const FPotionResult& PotionData) const;

	// Potion Spawning Functions
	UFUNCTION(BlueprintCallable, Category = "Potion Crafting|Spawning")
	bool SpawnPotionInWorld(const FPotionResult& PotionData, const FVector& SpawnLocation) const;

	UFUNCTION(BlueprintCallable, Category = "Potion Crafting|Spawning")
	UWitchPTInventoryItemInstance* AddPotionToInventory(const FPotionResult& PotionData, ACharacter* TargetCharacter) const;

	UFUNCTION(BlueprintCallable, Category = "Potion Crafting|Spawning")
	UWitchPTInventoryItemInstance* CreatePotionInstance(const FPotionResult& PotionData) const;

	// Main Crafting Function
	UFUNCTION(BlueprintCallable, Category = "Potion Crafting|Main", CallInEditor, meta = (DisplayName = "Craft Potion"))
	FPotionResult CraftPotion(bool bAddToInventory = true, ACharacter* TargetCharacter = nullptr, const FVector& WorldSpawnLocation = FVector::ZeroVector);

	// Crafting Helper Functions
	UFUNCTION(BlueprintCallable, Category = "Potion Crafting|Helpers")
	bool CanPerformCrafting() const;

	UFUNCTION(BlueprintCallable, Category = "Potion Crafting|Helpers")
	void ClearIngredientSlots();

	// Debug/Testing Helper Functions
	UFUNCTION(BlueprintCallable, Category = "Debug|Testing", CallInEditor, meta = (DisplayName = "Quick Test Craft"))
	void DebugTestCrafting();

	// Getters
	UWitchPTInventoryItemInstance* GetBaseIngredient() const;
	UWitchPTInventoryItemInstance* GetPrincipalIngredient() const;
	UWitchPTInventoryItemInstance* GetModifierIngredient() const;
	TSubclassOf<UUserWidget> GetBaseIngredientIcon() const;
	TSubclassOf<UUserWidget> GetPrincipalIngredientIcon() const;
	TSubclassOf<UUserWidget> GetModifierIngredientIcon() const;

protected:
	// Broadcast Helper Functions
	void BroadcastBaseIngredientDropped() const;
	void BroadcastBaseIngredientIconSet() const;
	void BroadcastPrincipalIngredientDropped() const;
	void BroadcastPrincipalIngredientIconSet() const;
	void BroadcastModifierIngredientDropped() const;
	void BroadcastModifierIngredientIconSet() const;

private:
	// Helper function to check if an ingredient has required crafting properties
	bool HasValidCraftingProperties(UWitchPTInventoryItemInstance* Ingredient) const;

	// Helper function to get the base potion definition from the owning cauldron
	TSubclassOf<UWitchPTInventoryItemDefinition> GetBasePotionItemDefinition() const;
};
