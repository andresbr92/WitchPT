// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/Components/CauldronCraftComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "FWitchPTGameplayTags.h"
#include "Inventory/WitchPTInventoryItemDefinition.h"
#include "Inventory/WitchPTInventoryItemInstance.h"
#include "Inventory/WitchPTInventoryManagerComponent.h"
#include "Inventory/Fragments/WitchPTInventoryFragment_UIDetails.h"
#include "Inventory/Fragments/WitchPTInventoryItemFragment_IngredientCraftingProperties.h"
#include "Inventory/Fragments/WitchPTInventoryItemFragment_PotionProperties.h"
#include "Inventory/Fragments/WitchPTInventoryItemFragment_WorldDetails.h"
#include "Player/WitchPTPlayerController.h"
#include "Item/CauldronAltar.h"
#include "Item/Potion/PotionBase.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UCauldronCraftComponent::UCauldronCraftComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);

	BaseIngredient = nullptr;
	PrincipalIngredient = nullptr;
	ModifierIngredient = nullptr;

	// Initialize default crafting recipe
	DefaultCraftingRecipe.bRequiresBaseIngredient = true;
	DefaultCraftingRecipe.bRequiresPrincipalIngredient = true;
	DefaultCraftingRecipe.bRequiresModifierIngredient = false;
}

// Called when the game starts
void UCauldronCraftComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UCauldronCraftComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCauldronCraftComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    DOREPLIFETIME(UCauldronCraftComponent, BaseIngredientIcon);
    DOREPLIFETIME(UCauldronCraftComponent, PrincipalIngredientIcon);
    DOREPLIFETIME(UCauldronCraftComponent, ModifierIngredientIcon);
    DOREPLIFETIME(UCauldronCraftComponent, CraftedPotion);
}

void UCauldronCraftComponent::OnRep_BaseIngredientIcon()
{
    BroadcastBaseIngredientIconSet();
}

void UCauldronCraftComponent::OnRep_PrincipalIngredientIcon()
{
    BroadcastPrincipalIngredientIconSet();
}

void UCauldronCraftComponent::OnRep_PotentiatorIngredientIcon()
{
    BroadcastModifierIngredientIconSet();
}

void UCauldronCraftComponent::OnRep_CraftedPotion()
{
    // Broadcast to UI or other systems that a potion has been crafted
    UE_LOG(LogTemp, Log, TEXT("UCauldronCraftComponent::OnRep_CraftedPotion: Potion crafted with validity: %s"), 
           CraftedPotion.bIsValid ? TEXT("Valid") : TEXT("Invalid"));
}

// ----------------------------------- CRAFTING VALIDATION FUNCTIONS ---------------------------------------------- //

bool UCauldronCraftComponent::CanCraftPotion() const
{
    const FPotionCraftingRecipe& Recipe = GetRequiredIngredients();
    
    // Check if required base ingredient is present
    if (Recipe.bRequiresBaseIngredient)
    {
        if (!BaseIngredient || !HasValidCraftingProperties(BaseIngredient))
        {
            UE_LOG(LogTemp, Warning, TEXT("UCauldronCraftComponent::CanCraftPotion: Base ingredient missing or invalid"));
            return false;
        }
    }
    
    // Check if required principal ingredient is present
    if (Recipe.bRequiresPrincipalIngredient)
    {
        if (!PrincipalIngredient || !HasValidCraftingProperties(PrincipalIngredient))
        {
            UE_LOG(LogTemp, Warning, TEXT("UCauldronCraftComponent::CanCraftPotion: Principal ingredient missing or invalid"));
            return false;
        }
    }
    
    // Check if required modifier ingredient is present (optional by default)
    if (Recipe.bRequiresModifierIngredient)
    {
        if (!ModifierIngredient || !HasValidCraftingProperties(ModifierIngredient))
        {
            UE_LOG(LogTemp, Warning, TEXT("UCauldronCraftComponent::CanCraftPotion: Modifier ingredient missing or invalid"));
            return false;
        }
    }
    
    // Validate ingredient combination compatibility
    if (!ValidateIngredientCombination())
    {
        UE_LOG(LogTemp, Warning, TEXT("UCauldronCraftComponent::CanCraftPotion: Ingredient combination is not valid"));
        return false;
    }
    
    UE_LOG(LogTemp, Log, TEXT("UCauldronCraftComponent::CanCraftPotion: All requirements met, can craft potion"));
    return true;
}

FPotionCraftingRecipe UCauldronCraftComponent::GetRequiredIngredients() const
{
    // For now, return the default recipe
    // This could be expanded later to support different recipes based on context
    return DefaultCraftingRecipe;
}

bool UCauldronCraftComponent::ValidateIngredientCombination() const
{
    const FPotionCraftingRecipe& Recipe = GetRequiredIngredients();
    
    // If we don't have the minimum required ingredients, combination is invalid
    if (Recipe.bRequiresBaseIngredient && !BaseIngredient)
    {
        return false;
    }
    
    if (Recipe.bRequiresPrincipalIngredient && !PrincipalIngredient)
    {
        return false;
    }
    
    // Check base liquid type compatibility if specified in recipe
    if (BaseIngredient && Recipe.AllowedBaseLiquidTypes.Num() > 0)
    {
        const UWitchPTInventoryItemFragment_IngredientCraftingProperties* BaseProps = 
            Cast<UWitchPTInventoryItemFragment_IngredientCraftingProperties>(
                BaseIngredient->FindFragmentByClass(UWitchPTInventoryItemFragment_IngredientCraftingProperties::StaticClass()));
        
        if (BaseProps && !Recipe.AllowedBaseLiquidTypes.HasTag(BaseProps->BaseIngredientData.BaseLiquidTypeTag))
        {
            UE_LOG(LogTemp, Warning, TEXT("UCauldronCraftComponent::ValidateIngredientCombination: Base liquid type not allowed"));
            return false;
        }
    }
    
    // Check principal essence compatibility if specified in recipe
    if (PrincipalIngredient && Recipe.AllowedPrincipalEssences.Num() > 0)
    {
        const UWitchPTInventoryItemFragment_IngredientCraftingProperties* PrincipalProps = 
            Cast<UWitchPTInventoryItemFragment_IngredientCraftingProperties>(
                PrincipalIngredient->FindFragmentByClass(UWitchPTInventoryItemFragment_IngredientCraftingProperties::StaticClass()));
        
        if (PrincipalProps && !Recipe.AllowedPrincipalEssences.HasTag(PrincipalProps->PrincipalIngredientData.PrincipalEssenceTag))
        {
            UE_LOG(LogTemp, Warning, TEXT("UCauldronCraftComponent::ValidateIngredientCombination: Principal essence not allowed"));
            return false;
        }
    }
    
    // Check for incompatible combinations
    if (Recipe.IncompatibleCombinations.Num() > 0)
    {
        // Build current combination tags
        FGameplayTagContainer CurrentCombination;
        
        if (BaseIngredient)
        {
            const UWitchPTInventoryItemFragment_IngredientCraftingProperties* BaseProps = 
                Cast<UWitchPTInventoryItemFragment_IngredientCraftingProperties>(
                    BaseIngredient->FindFragmentByClass(UWitchPTInventoryItemFragment_IngredientCraftingProperties::StaticClass()));
            if (BaseProps)
            {
                CurrentCombination.AddTag(BaseProps->BaseIngredientData.BaseLiquidTypeTag);
                CurrentCombination.AddTag(BaseProps->CategoryTag);
            }
        }
        
        if (PrincipalIngredient)
        {
            const UWitchPTInventoryItemFragment_IngredientCraftingProperties* PrincipalProps = 
                Cast<UWitchPTInventoryItemFragment_IngredientCraftingProperties>(
                    PrincipalIngredient->FindFragmentByClass(UWitchPTInventoryItemFragment_IngredientCraftingProperties::StaticClass()));
            if (PrincipalProps)
            {
                CurrentCombination.AddTag(PrincipalProps->PrincipalIngredientData.PrincipalEssenceTag);
                CurrentCombination.AddTag(PrincipalProps->CategoryTag);
            }
        }
        
        if (ModifierIngredient)
        {
            const UWitchPTInventoryItemFragment_IngredientCraftingProperties* ModifierProps = 
                Cast<UWitchPTInventoryItemFragment_IngredientCraftingProperties>(
                    ModifierIngredient->FindFragmentByClass(UWitchPTInventoryItemFragment_IngredientCraftingProperties::StaticClass()));
            if (ModifierProps)
            {
                CurrentCombination.AppendTags(ModifierProps->ModifierIngredientData.ModifierEffectTags);
                CurrentCombination.AddTag(ModifierProps->CategoryTag);
            }
        }
        
        // Check if any incompatible combination exists
        for (const FGameplayTag& IncompatibleTag : Recipe.IncompatibleCombinations)
        {
            if (CurrentCombination.HasTag(IncompatibleTag))
            {
                UE_LOG(LogTemp, Warning, TEXT("UCauldronCraftComponent::ValidateIngredientCombination: Found incompatible combination: %s"), 
                       *IncompatibleTag.ToString());
                return false;
            }
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("UCauldronCraftComponent::ValidateIngredientCombination: Ingredient combination is valid"));
    return true;
}

bool UCauldronCraftComponent::HasValidCraftingProperties(UWitchPTInventoryItemInstance* Ingredient) const
{
    if (!Ingredient)
    {
        return false;
    }
    
    const UWitchPTInventoryItemFragment_IngredientCraftingProperties* CraftingProps = 
        Cast<UWitchPTInventoryItemFragment_IngredientCraftingProperties>(
            Ingredient->FindFragmentByClass(UWitchPTInventoryItemFragment_IngredientCraftingProperties::StaticClass()));
    
    return CraftingProps != nullptr;
}

// ----------------------------------- DEBUG FUNCTIONS ---------------------------------------------- //

void UCauldronCraftComponent::PrintIngredientDebugData() const
{
    UE_LOG(LogTemp, Warning, TEXT("=== CAULDRON CRAFT COMPONENT DEBUG DATA ==="));
    UE_LOG(LogTemp, Warning, TEXT("Component Owner: %s"), GetOwner() ? *GetOwner()->GetName() : TEXT("None"));
    
    // Print Base Ingredient Data
    if (BaseIngredient)
    {
        UE_LOG(LogTemp, Warning, TEXT("--- BASE INGREDIENT SLOT ---"));
        PrintSpecificIngredientData(BaseIngredient, TEXT("Base"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("--- BASE INGREDIENT SLOT: EMPTY ---"));
    }
    
    // Print Principal Ingredient Data
    if (PrincipalIngredient)
    {
        UE_LOG(LogTemp, Warning, TEXT("--- PRINCIPAL INGREDIENT SLOT ---"));
        PrintSpecificIngredientData(PrincipalIngredient, TEXT("Principal"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("--- PRINCIPAL INGREDIENT SLOT: EMPTY ---"));
    }
    
    // Print Modifier Ingredient Data
    if (ModifierIngredient)
    {
        UE_LOG(LogTemp, Warning, TEXT("--- MODIFIER INGREDIENT SLOT ---"));
        PrintSpecificIngredientData(ModifierIngredient, TEXT("Modifier"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("--- MODIFIER INGREDIENT SLOT: EMPTY ---"));
    }
    
    // Print Crafting Status
    UE_LOG(LogTemp, Warning, TEXT("--- CRAFTING STATUS ---"));
    UE_LOG(LogTemp, Warning, TEXT("Can Craft Potion: %s"), CanCraftPotion() ? TEXT("YES") : TEXT("NO"));
    UE_LOG(LogTemp, Warning, TEXT("Ingredient Combination Valid: %s"), ValidateIngredientCombination() ? TEXT("YES") : TEXT("NO"));
    
    // Print Current Crafted Potion Status
    UE_LOG(LogTemp, Warning, TEXT("--- CURRENT CRAFTED POTION ---"));
    UE_LOG(LogTemp, Warning, TEXT("Crafted Potion Valid: %s"), CraftedPotion.bIsValid ? TEXT("YES") : TEXT("NO"));
    if (CraftedPotion.bIsValid)
    {
        UE_LOG(LogTemp, Warning, TEXT("Potion Essence: %s"), *CraftedPotion.PotionEssenceTag.ToString());
        UE_LOG(LogTemp, Warning, TEXT("Base Liquid: %s"), *CraftedPotion.BaseLiquidTypeTag.ToString());
        UE_LOG(LogTemp, Warning, TEXT("Final Potency: %.2f"), CraftedPotion.FinalPotency);
        UE_LOG(LogTemp, Warning, TEXT("Final Duration: %.2f"), CraftedPotion.FinalDuration);
        UE_LOG(LogTemp, Warning, TEXT("Final Charges: %d"), CraftedPotion.FinalCharges);
    }
    
    UE_LOG(LogTemp, Warning, TEXT("=== END DEBUG DATA ==="));
    CalculatePotionProperties();
}

void UCauldronCraftComponent::PrintSpecificIngredientData(UWitchPTInventoryItemInstance* Ingredient, const FString& SlotName) const
{
    if (!Ingredient)
    {
        UE_LOG(LogTemp, Warning, TEXT("%s Ingredient: NULL"), *SlotName);
        return;
    }
    
    // Print basic ingredient info
    UE_LOG(LogTemp, Warning, TEXT("%s Ingredient Definition: %s"), 
           *SlotName, 
           Ingredient->GetItemDef() ? *Ingredient->GetItemDef()->GetName() : TEXT("None"));
    UE_LOG(LogTemp, Warning, TEXT("%s Stack Count: %d"), *SlotName, Ingredient->GetTotalStackCount());
    
    // Get the crafting properties fragment
    const UWitchPTInventoryItemFragment_IngredientCraftingProperties* CraftingProps = 
        Cast<UWitchPTInventoryItemFragment_IngredientCraftingProperties>(
            Ingredient->FindFragmentByClass(UWitchPTInventoryItemFragment_IngredientCraftingProperties::StaticClass()));
    
    if (!CraftingProps)
    {
        UE_LOG(LogTemp, Error, TEXT("%s Ingredient: NO CRAFTING PROPERTIES FRAGMENT FOUND!"), *SlotName);
        return;
    }
    
    // Print common crafting properties
    UE_LOG(LogTemp, Warning, TEXT("%s Slot Usage Tag: %s"), *SlotName, *CraftingProps->SlotUsageTag.ToString());
    UE_LOG(LogTemp, Warning, TEXT("%s Tier Tag: %s"), *SlotName, *CraftingProps->TierTag.ToString());
    UE_LOG(LogTemp, Warning, TEXT("%s Category Tag: %s"), *SlotName, *CraftingProps->CategoryTag.ToString());
    
    // Print Base Ingredient specific data
    UE_LOG(LogTemp, Warning, TEXT("%s Base Liquid Type: %s"), 
           *SlotName, *CraftingProps->BaseIngredientData.BaseLiquidTypeTag.ToString());
    UE_LOG(LogTemp, Warning, TEXT("%s Default Charges: %d"), 
           *SlotName, CraftingProps->BaseIngredientData.DefaultChargesProvided);
    UE_LOG(LogTemp, Warning, TEXT("%s Stability Modifier: %.3f"), 
           *SlotName, CraftingProps->BaseIngredientData.StabilityModifier);
    
    // Print Principal Ingredient specific data
    UE_LOG(LogTemp, Warning, TEXT("%s Principal Essence: %s"), 
           *SlotName, *CraftingProps->PrincipalIngredientData.PrincipalEssenceTag.ToString());
    UE_LOG(LogTemp, Warning, TEXT("%s Base Potency: %.3f"), 
           *SlotName, CraftingProps->PrincipalIngredientData.BasePotency);
    UE_LOG(LogTemp, Warning, TEXT("%s Base Duration: %.3f"), 
           *SlotName, CraftingProps->PrincipalIngredientData.BaseDuration);
    UE_LOG(LogTemp, Warning, TEXT("%s Is Ritual Essence: %s"), 
           *SlotName, CraftingProps->PrincipalIngredientData.bIsRitualEssence ? TEXT("YES") : TEXT("NO"));
    
    // Print Modifier Ingredient specific data
    FString ModifierEffectTagsString;
    for (const FGameplayTag& Tag : CraftingProps->ModifierIngredientData.ModifierEffectTags)
    {
        if (!ModifierEffectTagsString.IsEmpty())
        {
            ModifierEffectTagsString += TEXT(", ");
        }
        ModifierEffectTagsString += Tag.ToString();
    }
    UE_LOG(LogTemp, Warning, TEXT("%s Modifier Effect Tags: [%s]"), *SlotName, *ModifierEffectTagsString);
    UE_LOG(LogTemp, Warning, TEXT("%s Potency Modification: %.3f"), 
           *SlotName, CraftingProps->ModifierIngredientData.PotencyModificationValue);
    UE_LOG(LogTemp, Warning, TEXT("%s Duration Modification: %.3f"), 
           *SlotName, CraftingProps->ModifierIngredientData.DurationModificationValue);
    UE_LOG(LogTemp, Warning, TEXT("%s Granted Property Tag: %s"), 
           *SlotName, *CraftingProps->ModifierIngredientData.GrantedPropertyTag.ToString());
    UE_LOG(LogTemp, Warning, TEXT("%s Added Effect Tag: %s"), 
           *SlotName, *CraftingProps->ModifierIngredientData.AddedEffectTag.ToString());
    UE_LOG(LogTemp, Warning, TEXT("%s Modifier Strength: %.3f"), 
           *SlotName, CraftingProps->ModifierIngredientData.StrengthOfModifier);
}

void UCauldronCraftComponent::TrySetIngredientInSlot(const ACharacter* RequestingCharacter, const TSubclassOf<UWitchPTInventoryItemDefinition>& IngredientItemDef)
{
    if (!GetOwner()->HasAuthority())
    {
        UE_LOG(LogTemp, Warning, TEXT("UCauldronCraftComponent::TrySetIngredientInSlot: Not authority"));
        return;
    }

    AWitchPTPlayerController* PC = Cast<AWitchPTPlayerController>(RequestingCharacter->GetController());
    if (!PC)
    {
        UE_LOG(LogTemp, Error, TEXT("UCauldronCraftComponent::TrySetIngredientInSlot: RequestingCharacter does not have a valid PlayerController."));
        return;
    }

    UWitchPTInventoryManagerComponent* InventoryManager = PC->GetInventoryManager();
    if (!InventoryManager)
    {
        UE_LOG(LogTemp, Error, TEXT("UCauldronCraftComponent::TrySetIngredientInSlot: Could not get InventoryManager from PlayerController."));
        return;
    }
    
    UWitchPTInventoryItemInstance* InstanceFromInventory = InventoryManager->FindFirstItemStackByDefinition(IngredientItemDef);

    if (!InstanceFromInventory)
    {
        UE_LOG(LogTemp, Warning, TEXT("UCauldronCraftComponent::TrySetIngredientInSlot: ItemDefinition %s not found in inventory for %s."), *IngredientItemDef->GetName(), *RequestingCharacter->GetName());
        return;
    }
    
    const UWitchPTInventoryItemFragment_IngredientCraftingProperties* IngredientCraftingDetails = Cast<UWitchPTInventoryItemFragment_IngredientCraftingProperties>(InstanceFromInventory->FindFragmentByClass(UWitchPTInventoryItemFragment_IngredientCraftingProperties::StaticClass()));
    if (!IngredientCraftingDetails)
    {
        UE_LOG(LogTemp, Warning, TEXT("UCauldronCraftComponent::TrySetIngredientInSlot: IngredientCraftingDetails is not valid for %s."), *RequestingCharacter->GetName());
        return;
    }

    // Determine ingredient type based on SlotUsageTag
    const FWitchPTGameplayTags& GameplayTags = FWitchPTGameplayTags::Get();
    bool bIsBaseIngredient = IngredientCraftingDetails->SlotUsageTag.MatchesTag(GameplayTags.Item_Consumable_Ingredient_CanBeUsedIn_BaseSlot);
    bool bIsPrincipalIngredient = IngredientCraftingDetails->SlotUsageTag.MatchesTag(GameplayTags.Item_Consumable_Ingredient_CanBeUsedIn_PrincipalSlot);
    bool bIsModifierIngredient = IngredientCraftingDetails->SlotUsageTag.MatchesTag(GameplayTags.Item_Consumable_Ingredient_CanBeUsedIn_ModifierSlot);

    // Check if the appropriate slot is already occupied
    if (bIsBaseIngredient && BaseIngredient != nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("UCauldronCraftComponent::TrySetIngredientInSlot: Base ingredient slot is already occupied"));
        return;
    }
    
    if (bIsPrincipalIngredient && PrincipalIngredient != nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("UCauldronCraftComponent::TrySetIngredientInSlot: Principal ingredient slot is already occupied"));
        return;
    }
    
    if (bIsModifierIngredient && ModifierIngredient != nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("UCauldronCraftComponent::TrySetIngredientInSlot: Modifier ingredient slot is already occupied"));
        return;
    }

    // If none of the slot usage tags match, this ingredient can't be used
    if (!bIsBaseIngredient && !bIsPrincipalIngredient && !bIsModifierIngredient)
    {
        UE_LOG(LogTemp, Warning, TEXT("UCauldronCraftComponent::TrySetIngredientInSlot: Ingredient %s doesn't have a valid slot usage tag"), *IngredientItemDef->GetName());
        return;
    }
    
    bool bConsumedSuccessfully = false;
    int32 CurrentStackCount = InstanceFromInventory->GetTotalStackCount();
    
    if (CurrentStackCount > 0)
    {
        UWitchPTInventoryItemInstance* IngredientToSet = nullptr;
        
        if (CurrentStackCount == 1)
        {
            IngredientToSet = InstanceFromInventory;
            InventoryManager->Server_RemoveItemInstance(InstanceFromInventory);
            bConsumedSuccessfully = true;
        }
        else
        {
            InventoryManager->Server_UpdateItemStackCount(InstanceFromInventory, CurrentStackCount - 1);
            IngredientToSet = InstanceFromInventory;
            bConsumedSuccessfully = true;
        }
        
        // Get UI fragment for icon
        const UWitchPTInventoryFragment_UIDetails* UIFragment = Cast<UWitchPTInventoryFragment_UIDetails>(InstanceFromInventory->FindFragmentByClass(UWitchPTInventoryFragment_UIDetails::StaticClass()));
        TSubclassOf<UUserWidget> IngredientIcon = nullptr;
        if (IsValid(UIFragment))
        {
            IngredientIcon = UIFragment->IconWidget;
        }
        
        // Set the ingredient in the appropriate slot
        if (bIsBaseIngredient)
        {
            BaseIngredient = IngredientToSet;
            BaseIngredientIcon = IngredientIcon;
            UE_LOG(LogTemp, Log, TEXT("UCauldronCraftComponent::TrySetIngredientInSlot: Base ingredient set successfully"));
            
            if (GetOwner()->HasAuthority())
            {
                BroadcastBaseIngredientIconSet();
            }
        }
        else if (bIsPrincipalIngredient)
        {
            PrincipalIngredient = IngredientToSet;
            PrincipalIngredientIcon = IngredientIcon;
            UE_LOG(LogTemp, Log, TEXT("UCauldronCraftComponent::TrySetIngredientInSlot: Principal ingredient set successfully"));
            
            if (GetOwner()->HasAuthority())
            {
                BroadcastPrincipalIngredientIconSet();
            }
        }
        else if (bIsModifierIngredient)
        {
            ModifierIngredient = IngredientToSet;
            ModifierIngredientIcon = IngredientIcon;
            UE_LOG(LogTemp, Log, TEXT("UCauldronCraftComponent::TrySetIngredientInSlot: Modifier ingredient set successfully"));
            
            if (GetOwner()->HasAuthority())
            {
                BroadcastModifierIngredientIconSet();
            }
        }
    }
}

// ----------------------------------- BROADCAST HELPER FUNCTIONS ---------------------------------------------- //
void UCauldronCraftComponent::BroadcastBaseIngredientDropped() const
{
    OnBaseIngredientSetDelegate.Broadcast(BaseIngredient);
}

void UCauldronCraftComponent::BroadcastBaseIngredientIconSet() const
{
    OnBaseIngredientIconSetDelegate.Broadcast(BaseIngredientIcon);
}

void UCauldronCraftComponent::BroadcastPrincipalIngredientDropped() const
{
    OnPrincipalIngredientSetDelegate.Broadcast(PrincipalIngredient);
}

void UCauldronCraftComponent::BroadcastPrincipalIngredientIconSet() const
{
    OnPrincipalIngredientIconSetDelegate.Broadcast(PrincipalIngredientIcon);
}

void UCauldronCraftComponent::BroadcastModifierIngredientDropped() const
{
    OnModifierIngredientSetDelegate.Broadcast(ModifierIngredient);
}

void UCauldronCraftComponent::BroadcastModifierIngredientIconSet() const
{
    OnModifierIngredientIconSetDelegate.Broadcast(ModifierIngredientIcon);
}

UWitchPTInventoryItemInstance* UCauldronCraftComponent::GetBaseIngredient() const
{
    return BaseIngredient;
}

UWitchPTInventoryItemInstance* UCauldronCraftComponent::GetPrincipalIngredient() const
{
    return PrincipalIngredient;
}

UWitchPTInventoryItemInstance* UCauldronCraftComponent::GetModifierIngredient() const
{
    return ModifierIngredient;
}

// ----------------------------------- INGREDIENT DATA EXTRACTION FUNCTIONS ---------------------------------------------- //

FExtractedBaseIngredientData UCauldronCraftComponent::ExtractBaseIngredientProperties() const
{
    FExtractedBaseIngredientData ExtractedData;
    
    if (!BaseIngredient)
    {
        UE_LOG(LogTemp, Warning, TEXT("UCauldronCraftComponent::ExtractBaseIngredientProperties: No base ingredient found"));
        return ExtractedData; // bIsValid remains false
    }
    
    const UWitchPTInventoryItemFragment_IngredientCraftingProperties* CraftingProps = 
        Cast<UWitchPTInventoryItemFragment_IngredientCraftingProperties>(
            BaseIngredient->FindFragmentByClass(UWitchPTInventoryItemFragment_IngredientCraftingProperties::StaticClass()));
    
    if (!CraftingProps)
    {
        UE_LOG(LogTemp, Error, TEXT("UCauldronCraftComponent::ExtractBaseIngredientProperties: No crafting properties found"));
        return ExtractedData;
    }
    
    // Extract base ingredient data
    ExtractedData.BaseLiquidTypeTag = CraftingProps->BaseIngredientData.BaseLiquidTypeTag;
    ExtractedData.DefaultChargesProvided = CraftingProps->BaseIngredientData.DefaultChargesProvided;
    ExtractedData.StabilityModifier = CraftingProps->BaseIngredientData.StabilityModifier;
    ExtractedData.TierTag = CraftingProps->TierTag;
    ExtractedData.bIsValid = true;
    
    UE_LOG(LogTemp, Log, TEXT("UCauldronCraftComponent::ExtractBaseIngredientProperties: Successfully extracted - Liquid: %s, Charges: %d, Stability: %.3f"), 
           *ExtractedData.BaseLiquidTypeTag.ToString(), 
           ExtractedData.DefaultChargesProvided, 
           ExtractedData.StabilityModifier);
    
    return ExtractedData;
}

FExtractedPrincipalIngredientData UCauldronCraftComponent::ExtractPrincipalIngredientProperties() const
{
    FExtractedPrincipalIngredientData ExtractedData;
    
    if (!PrincipalIngredient)
    {
        UE_LOG(LogTemp, Warning, TEXT("UCauldronCraftComponent::ExtractPrincipalIngredientProperties: No principal ingredient found"));
        return ExtractedData; // bIsValid remains false
    }
    
    const UWitchPTInventoryItemFragment_IngredientCraftingProperties* CraftingProps = 
        Cast<UWitchPTInventoryItemFragment_IngredientCraftingProperties>(
            PrincipalIngredient->FindFragmentByClass(UWitchPTInventoryItemFragment_IngredientCraftingProperties::StaticClass()));
    
    if (!CraftingProps)
    {
        UE_LOG(LogTemp, Error, TEXT("UCauldronCraftComponent::ExtractPrincipalIngredientProperties: No crafting properties found"));
        return ExtractedData;
    }
    
    // Extract principal ingredient data
    ExtractedData.PrincipalEssenceTag = CraftingProps->PrincipalIngredientData.PrincipalEssenceTag;
    ExtractedData.BasePotency = CraftingProps->PrincipalIngredientData.BasePotency;
    ExtractedData.BaseDuration = CraftingProps->PrincipalIngredientData.BaseDuration;
    ExtractedData.bIsRitualEssence = CraftingProps->PrincipalIngredientData.bIsRitualEssence;
    ExtractedData.TierTag = CraftingProps->TierTag;
    ExtractedData.bIsValid = true;
    
    UE_LOG(LogTemp, Log, TEXT("UCauldronCraftComponent::ExtractPrincipalIngredientProperties: Successfully extracted - Essence: %s, Potency: %.3f, Duration: %.3f"), 
           *ExtractedData.PrincipalEssenceTag.ToString(), 
           ExtractedData.BasePotency, 
           ExtractedData.BaseDuration);
    
    return ExtractedData;
}

FExtractedModifierIngredientData UCauldronCraftComponent::ExtractModifierIngredientProperties() const
{
    FExtractedModifierIngredientData ExtractedData;
    
    if (!ModifierIngredient)
    {
        UE_LOG(LogTemp, Log, TEXT("UCauldronCraftComponent::ExtractModifierIngredientProperties: No modifier ingredient found (this is optional)"));
        return ExtractedData; // bIsValid remains false, but this is acceptable for optional ingredients
    }
    
    const UWitchPTInventoryItemFragment_IngredientCraftingProperties* CraftingProps = 
        Cast<UWitchPTInventoryItemFragment_IngredientCraftingProperties>(
            ModifierIngredient->FindFragmentByClass(UWitchPTInventoryItemFragment_IngredientCraftingProperties::StaticClass()));
    
    if (!CraftingProps)
    {
        UE_LOG(LogTemp, Error, TEXT("UCauldronCraftComponent::ExtractModifierIngredientProperties: No crafting properties found"));
        return ExtractedData;
    }
    
    // Extract modifier ingredient data
    ExtractedData.ModifierEffectTags = CraftingProps->ModifierIngredientData.ModifierEffectTags;
    ExtractedData.PotencyModificationValue = CraftingProps->ModifierIngredientData.PotencyModificationValue;
    ExtractedData.DurationModificationValue = CraftingProps->ModifierIngredientData.DurationModificationValue;
    ExtractedData.GrantedPropertyTag = CraftingProps->ModifierIngredientData.GrantedPropertyTag;
    ExtractedData.AddedEffectTag = CraftingProps->ModifierIngredientData.AddedEffectTag;
    ExtractedData.StrengthOfModifier = CraftingProps->ModifierIngredientData.StrengthOfModifier;
    ExtractedData.TierTag = CraftingProps->TierTag;
    ExtractedData.bIsValid = true;
    
    UE_LOG(LogTemp, Log, TEXT("UCauldronCraftComponent::ExtractModifierIngredientProperties: Successfully extracted - Effects: %d, Potency Mod: %.3f, Duration Mod: %.3f"), 
           ExtractedData.ModifierEffectTags.Num(), 
           ExtractedData.PotencyModificationValue, 
           ExtractedData.DurationModificationValue);
    
    return ExtractedData;
}

// ----------------------------------- POTION CALCULATION FUNCTIONS ---------------------------------------------- //

FPotionResult UCauldronCraftComponent::CalculatePotionProperties() const
{
    FPotionResult PotionResult;
    
    // Check if we can craft a potion
    if (!CanCraftPotion())
    {
        UE_LOG(LogTemp, Warning, TEXT("UCauldronCraftComponent::CalculatePotionProperties: Cannot craft potion with current ingredients"));
        return PotionResult; // bIsValid remains false
    }
    
    // Extract ingredient data
    FExtractedBaseIngredientData BaseData = ExtractBaseIngredientProperties();
    FExtractedPrincipalIngredientData PrincipalData = ExtractPrincipalIngredientProperties();
    FExtractedModifierIngredientData ModifierData = ExtractModifierIngredientProperties();
    
    // Validate extracted data
    if (!BaseData.bIsValid || !PrincipalData.bIsValid)
    {
        UE_LOG(LogTemp, Error, TEXT("UCauldronCraftComponent::CalculatePotionProperties: Required ingredient data is invalid"));
        return PotionResult;
    }
    
    UE_LOG(LogTemp, Log, TEXT("UCauldronCraftComponent::CalculatePotionProperties: Starting potion calculation..."));
    
    // Step 1: Start with base liquid properties
    PotionResult.BaseLiquidTypeTag = BaseData.BaseLiquidTypeTag;
    PotionResult.FinalCharges = BaseData.DefaultChargesProvided;
    float StabilityFactor = BaseData.StabilityModifier;
    
    // Step 2: Apply principal ingredient effects
    PotionResult.PotionEssenceTag = PrincipalData.PrincipalEssenceTag;
    PotionResult.FinalPotency = PrincipalData.BasePotency;
    PotionResult.FinalDuration = PrincipalData.BaseDuration;
    
    // Add principal essence to granted effects
    if (PrincipalData.PrincipalEssenceTag.IsValid())
    {
        PotionResult.GrantedEffectTags.AddTag(PrincipalData.PrincipalEssenceTag);
    }
    
    // Step 3: Apply modifier ingredient effects (if present)
    if (ModifierData.bIsValid)
    {
        UE_LOG(LogTemp, Log, TEXT("UCauldronCraftComponent::CalculatePotionProperties: Applying modifier effects..."));
        
        // Apply potency modification (multiplicative)
        PotionResult.FinalPotency *= ModifierData.PotencyModificationValue;
        
        // Apply duration modification (additive)
        PotionResult.FinalDuration += ModifierData.DurationModificationValue;
        
        // Add modifier effects to granted effects
        PotionResult.GrantedEffectTags.AppendTags(ModifierData.ModifierEffectTags);
        
        // Add granted property
        if (ModifierData.GrantedPropertyTag.IsValid())
        {
            PotionResult.GrantedPropertyTags.AddTag(ModifierData.GrantedPropertyTag);
        }
        
        // Add additional effect
        if (ModifierData.AddedEffectTag.IsValid())
        {
            PotionResult.GrantedEffectTags.AddTag(ModifierData.AddedEffectTag);
        }
        
        // Apply modifier strength to overall potency (additional multiplicative factor)
        PotionResult.FinalPotency *= ModifierData.StrengthOfModifier;
    }
    
    // Step 4: Apply stability modifier to final potency
    PotionResult.FinalPotency *= StabilityFactor;
    
    // Step 5: Calculate final potion tier
    PotionResult.PotionTierTag = CalculatePotionTier(BaseData, PrincipalData, ModifierData);
    
    // Step 6: Ensure minimum values
    PotionResult.FinalPotency = FMath::Max(PotionResult.FinalPotency, 0.1f); // Minimum potency
    PotionResult.FinalDuration = FMath::Max(PotionResult.FinalDuration, 1.0f); // Minimum duration
    PotionResult.FinalCharges = FMath::Max(PotionResult.FinalCharges, 1); // Minimum charges
    
    // Mark as valid
    PotionResult.bIsValid = true;
    
    UE_LOG(LogTemp, Log, TEXT("UCauldronCraftComponent::CalculatePotionProperties: Potion calculation complete - Potency: %.2f, Duration: %.2f, Charges: %d"), 
           PotionResult.FinalPotency, 
           PotionResult.FinalDuration, 
           PotionResult.FinalCharges);
    
    return PotionResult;
}

FGameplayTag UCauldronCraftComponent::CalculatePotionTier(const FExtractedBaseIngredientData& BaseData, 
                                                         const FExtractedPrincipalIngredientData& PrincipalData, 
                                                         const FExtractedModifierIngredientData& ModifierData) const
{
    // Simple tier calculation: use the highest tier among all ingredients
    // This could be expanded to use more complex logic later
    
    TArray<FGameplayTag> TierTags;
    
    if (BaseData.bIsValid && BaseData.TierTag.IsValid())
    {
        TierTags.Add(BaseData.TierTag);
    }
    
    if (PrincipalData.bIsValid && PrincipalData.TierTag.IsValid())
    {
        TierTags.Add(PrincipalData.TierTag);
    }
    
    if (ModifierData.bIsValid && ModifierData.TierTag.IsValid())
    {
        TierTags.Add(ModifierData.TierTag);
    }
    
    // For now, return the principal ingredient's tier as the base
    // This could be enhanced to compare tier values and return the highest
    if (PrincipalData.bIsValid && PrincipalData.TierTag.IsValid())
    {
        UE_LOG(LogTemp, Log, TEXT("UCauldronCraftComponent::CalculatePotionTier: Using principal ingredient tier: %s"), 
               *PrincipalData.TierTag.ToString());
        return PrincipalData.TierTag;
    }
    
    // Fallback to base ingredient tier
    if (BaseData.bIsValid && BaseData.TierTag.IsValid())
    {
        UE_LOG(LogTemp, Log, TEXT("UCauldronCraftComponent::CalculatePotionTier: Using base ingredient tier: %s"), 
               *BaseData.TierTag.ToString());
        return BaseData.TierTag;
    }
    
    // No valid tier found
    UE_LOG(LogTemp, Warning, TEXT("UCauldronCraftComponent::CalculatePotionTier: No valid tier tags found, returning empty tag"));
    return FGameplayTag();
}

// ----------------------------------- POTION ITEMDEFINITION GENERATION FUNCTIONS ---------------------------------------------- //

TSubclassOf<UWitchPTInventoryItemDefinition> UCauldronCraftComponent::GeneratePotionItemDefinition(const FPotionResult& PotionData) const
{
    if (!PotionData.bIsValid)
    {
        UE_LOG(LogTemp, Error, TEXT("UCauldronCraftComponent::GeneratePotionItemDefinition: Invalid potion data provided"));
        return nullptr;
    }

    TSubclassOf<UWitchPTInventoryItemDefinition> BasePotionItemDefinition = GetBasePotionItemDefinition();
    if (!BasePotionItemDefinition)
    {
        UE_LOG(LogTemp, Error, TEXT("UCauldronCraftComponent::GeneratePotionItemDefinition: BasePotionItemDefinition is not set"));
        return nullptr;
    }

    // For now, return the base definition
    // In a full implementation, you might want to create dynamic classes or use a different approach
    // This is a simplified approach where we'll set properties at runtime
    UE_LOG(LogTemp, Log, TEXT("UCauldronCraftComponent::GeneratePotionItemDefinition: Returning base potion definition for essence: %s"), 
           *PotionData.PotionEssenceTag.ToString());
    
    return BasePotionItemDefinition;
}

UWitchPTInventoryItemDefinition* UCauldronCraftComponent::CreatePotionItemDefinitionInstance(const FPotionResult& PotionData) const
{
    if (!PotionData.bIsValid)
    {
        UE_LOG(LogTemp, Error, TEXT("UCauldronCraftComponent::CreatePotionItemDefinitionInstance: Invalid potion data provided"));
        return nullptr;
    }

    TSubclassOf<UWitchPTInventoryItemDefinition> BasePotionItemDefinition = GetBasePotionItemDefinition();
    if (!BasePotionItemDefinition)
    {
        UE_LOG(LogTemp, Error, TEXT("UCauldronCraftComponent::CreatePotionItemDefinitionInstance: BasePotionItemDefinition is not set"));
        return nullptr;
    }

    // Create a new instance of the base potion definition
    UWitchPTInventoryItemDefinition* PotionDefinition = NewObject<UWitchPTInventoryItemDefinition>(
        GetTransientPackage(), 
        BasePotionItemDefinition
    );

    if (!PotionDefinition)
    {
        UE_LOG(LogTemp, Error, TEXT("UCauldronCraftComponent::CreatePotionItemDefinitionInstance: Failed to create potion definition instance"));
        return nullptr;
    }

    // Find or create the potion properties fragment
    UWitchPTInventoryItemFragment_PotionProperties* PotionPropsFragment = nullptr;
    
    // Check if base definition already has a potion properties fragment
    const UWitchPTInventoryItemFragment_PotionProperties* ExistingFragment = 
        Cast<UWitchPTInventoryItemFragment_PotionProperties>(
            PotionDefinition->FindFragmentByClass(UWitchPTInventoryItemFragment_PotionProperties::StaticClass()));

    if (ExistingFragment)
    {
        // Create a new fragment based on the existing one
        PotionPropsFragment = NewObject<UWitchPTInventoryItemFragment_PotionProperties>(PotionDefinition);
        
        // Copy any default values from the existing fragment if needed
        // For now, we'll just set the new properties
    }
    else
    {
        // Create a new potion properties fragment
        PotionPropsFragment = NewObject<UWitchPTInventoryItemFragment_PotionProperties>(PotionDefinition);
        
        // Add the fragment to the definition's fragments array
        PotionDefinition->Fragments.Add(PotionPropsFragment);
    }

    // Set the potion properties from the crafting result
    if (PotionPropsFragment)
    {
        PotionPropsFragment->SetPotionProperties(
            PotionData.PotionEssenceTag,
            PotionData.BaseLiquidTypeTag,
            PotionData.FinalPotency,
            PotionData.FinalDuration,
            PotionData.FinalCharges,
            PotionData.PotionTierTag,
            PotionData.GrantedEffectTags,
            PotionData.GrantedPropertyTags
        );

        UE_LOG(LogTemp, Log, TEXT("UCauldronCraftComponent::CreatePotionItemDefinitionInstance: Created potion definition with properties - %s"), 
               *PotionData.PotionEssenceTag.ToString());
    }

    return PotionDefinition;
}

// ----------------------------------- POTION SPAWNING FUNCTIONS ---------------------------------------------- //

UWitchPTInventoryItemInstance* UCauldronCraftComponent::CreatePotionInstance(const FPotionResult& PotionData) const
{
    if (!PotionData.bIsValid)
    {
        UE_LOG(LogTemp, Error, TEXT("UCauldronCraftComponent::CreatePotionInstance: Invalid potion data provided"));
        return nullptr;
    }

    // Create the potion definition instance
    UWitchPTInventoryItemDefinition* PotionDefinition = CreatePotionItemDefinitionInstance(PotionData);
    if (!PotionDefinition)
    {
        UE_LOG(LogTemp, Error, TEXT("UCauldronCraftComponent::CreatePotionInstance: Failed to create potion definition"));
        return nullptr;
    }

    // Create a new inventory item instance
    UWitchPTInventoryItemInstance* PotionInstance = NewObject<UWitchPTInventoryItemInstance>();
    if (!PotionInstance)
    {
        UE_LOG(LogTemp, Error, TEXT("UCauldronCraftComponent::CreatePotionInstance: Failed to create potion instance"));
        return nullptr;
    }

    // Set the item definition and stack count
    PotionInstance->SetItemDef(PotionDefinition->GetClass());
    PotionInstance->SetTotalStackCount(PotionData.FinalCharges);

    UE_LOG(LogTemp, Log, TEXT("UCauldronCraftComponent::CreatePotionInstance: Successfully created potion instance - %s with %d charges"), 
           *PotionData.PotionEssenceTag.ToString(), 
           PotionData.FinalCharges);

    return PotionInstance;
}

bool UCauldronCraftComponent::SpawnPotionInWorld(const FPotionResult& PotionData, const FVector& SpawnLocation) const
{
    if (!PotionData.bIsValid)
    {
        UE_LOG(LogTemp, Error, TEXT("UCauldronCraftComponent::SpawnPotionInWorld: Invalid potion data provided"));
        return false;
    }

    // Create the potion definition instance with proper properties
    UWitchPTInventoryItemDefinition* PotionDefinition = CreatePotionItemDefinitionInstance(PotionData);
    if (!PotionDefinition)
    {
        UE_LOG(LogTemp, Error, TEXT("UCauldronCraftComponent::SpawnPotionInWorld: Failed to create potion definition"));
        return false;
    }

    // Get the world for spawning
    UWorld* World = GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("UCauldronCraftComponent::SpawnPotionInWorld: No valid world found"));
        return false;
    }

    // Spawn PotionBase actor at the specified location
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    
    APotionBase* PotionActor = World->SpawnActor<APotionBase>(SpawnLocation, FRotator::ZeroRotator, SpawnParams);
    if (!PotionActor)
    {
        UE_LOG(LogTemp, Error, TEXT("UCauldronCraftComponent::SpawnPotionInWorld: Failed to spawn PotionBase actor"));
        return false;
    }

    // Set up the static mesh from the world details fragment
    const UWitchPTInventoryItemFragment_WorldDetails* WorldDetails = 
        Cast<UWitchPTInventoryItemFragment_WorldDetails>(
            PotionDefinition->FindFragmentByClass(UWitchPTInventoryItemFragment_WorldDetails::StaticClass()));

    if (WorldDetails && WorldDetails->HasValidWorldMesh())
    {
        // Get or create the static mesh component from PotionBase
        UStaticMeshComponent* MeshComponent = PotionActor->FindComponentByClass<UStaticMeshComponent>();
        
        if (!MeshComponent)
        {
            // Create a new StaticMeshComponent if one doesn't exist
            MeshComponent = NewObject<UStaticMeshComponent>(PotionActor);
            MeshComponent->SetupAttachment(PotionActor->GetRootComponent());
            MeshComponent->RegisterComponent();
            
            UE_LOG(LogTemp, Log, TEXT("UCauldronCraftComponent::SpawnPotionInWorld: Created new StaticMeshComponent for PotionBase"));
        }
        
        if (MeshComponent)
        {
            // Set the mesh from the world details fragment
            MeshComponent->SetStaticMesh(WorldDetails->WorldStaticMesh);
            
            // Apply scale if specified
            if (WorldDetails->WorldScale != FVector::OneVector)
            {
                MeshComponent->SetWorldScale3D(WorldDetails->WorldScale);
            }
            
            // Apply material override if specified
            if (WorldDetails->WorldMaterial)
            {
                MeshComponent->SetMaterial(0, WorldDetails->WorldMaterial);
            }
            
            // Set collision profile if specified
            if (!WorldDetails->CollisionProfileName.IsNone())
            {
                MeshComponent->SetCollisionProfileName(WorldDetails->CollisionProfileName);
            }

            UE_LOG(LogTemp, Log, TEXT("UCauldronCraftComponent::SpawnPotionInWorld: Applied mesh '%s' from WorldDetails fragment"), 
                   *WorldDetails->WorldStaticMesh->GetName());
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("UCauldronCraftComponent::SpawnPotionInWorld: Failed to create or find StaticMeshComponent"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("UCauldronCraftComponent::SpawnPotionInWorld: No WorldDetails fragment or invalid mesh - using default appearance"));
    }

    // Set the item definition in the potion actor
    // Note: You may need to add a SetItemDefinition method to PotionBase if it doesn't exist
    // For now, we'll log that the potion was spawned successfully
    
    UE_LOG(LogTemp, Log, TEXT("UCauldronCraftComponent::SpawnPotionInWorld: Successfully spawned potion at location: %s"), 
           *SpawnLocation.ToString());
    UE_LOG(LogTemp, Log, TEXT("UCauldronCraftComponent::SpawnPotionInWorld: Potion: %s with %d charges, %.2f potency"), 
           *PotionData.PotionEssenceTag.ToString(), 
           PotionData.FinalCharges,
           PotionData.FinalPotency);
    
    return true;
}

UWitchPTInventoryItemInstance* UCauldronCraftComponent::AddPotionToInventory(const FPotionResult& PotionData, ACharacter* TargetCharacter) const
{
    if (!TargetCharacter)
    {
        UE_LOG(LogTemp, Error, TEXT("UCauldronCraftComponent::AddPotionToInventory: TargetCharacter is null"));
        return nullptr;
    }

    UWitchPTInventoryItemInstance* PotionInstance = CreatePotionInstance(PotionData);
    if (!PotionInstance)
    {
        UE_LOG(LogTemp, Error, TEXT("UCauldronCraftComponent::AddPotionToInventory: Failed to create potion instance"));
        return nullptr;
    }

    // Get the player controller and inventory manager
    AWitchPTPlayerController* PC = Cast<AWitchPTPlayerController>(TargetCharacter->GetController());
    if (!PC)
    {
        UE_LOG(LogTemp, Error, TEXT("UCauldronCraftComponent::AddPotionToInventory: TargetCharacter does not have a valid PlayerController"));
        return nullptr;
    }

    UWitchPTInventoryManagerComponent* InventoryManager = PC->GetInventoryManager();
    if (!InventoryManager)
    {
        UE_LOG(LogTemp, Error, TEXT("UCauldronCraftComponent::AddPotionToInventory: Could not get InventoryManager from PlayerController"));
        return nullptr;
    }

    // Add the potion to inventory
    // bool bAddedSuccessfully = InventoryManager->Server_AddItem(PotionInstance);
    // if (bAddedSuccessfully)
    // {
    //     UE_LOG(LogTemp, Log, TEXT("UCauldronCraftComponent::AddPotionToInventory: Successfully added potion to inventory - %s"), 
    //            *PotionData.PotionEssenceTag.ToString());
    //     return PotionInstance;
    // }
    // else
    // {
    //     UE_LOG(LogTemp, Warning, TEXT("UCauldronCraftComponent::AddPotionToInventory: Failed to add potion to inventory"));
    //     return nullptr;
    // }
    return nullptr;
}

// ----------------------------------- MAIN CRAFTING FUNCTION ---------------------------------------------- //

FPotionResult UCauldronCraftComponent::CraftPotion(bool bAddToInventory, ACharacter* TargetCharacter, const FVector& WorldSpawnLocation)
{
    FPotionResult EmptyResult;
    
    // Only allow crafting on the server
    if (!GetOwner()->HasAuthority())
    {
        UE_LOG(LogTemp, Warning, TEXT("UCauldronCraftComponent::CraftPotion: Not authority - crafting not allowed"));
        return EmptyResult;
    }

    // Check if we can perform crafting
    if (!CanPerformCrafting())
    {
        UE_LOG(LogTemp, Warning, TEXT("UCauldronCraftComponent::CraftPotion: Cannot perform crafting with current setup"));
        return EmptyResult;
    }

    UE_LOG(LogTemp, Log, TEXT("UCauldronCraftComponent::CraftPotion: Starting potion crafting process..."));

    // Step 1: Calculate potion properties
    FPotionResult CraftingResult = CalculatePotionProperties();
    if (!CraftingResult.bIsValid)
    {
        UE_LOG(LogTemp, Error, TEXT("UCauldronCraftComponent::CraftPotion: Failed to calculate valid potion properties"));
        return EmptyResult;
    }

    // Step 2: Create and spawn/add the potion
    bool bPotionCreatedSuccessfully = false;
    
    if (bAddToInventory)
    {
        // Determine target character
        ACharacter* Target = TargetCharacter;
        if (!Target)
        {
            // Try to get character from owner (if cauldron is owned by a character)
            Target = Cast<ACharacter>(GetOwner());
            if (!Target)
            {
                // Try to get the closest player character (fallback)
                UE_LOG(LogTemp, Warning, TEXT("UCauldronCraftComponent::CraftPotion: No target character specified for inventory addition"));
                return EmptyResult;
            }
        }
        
        UWitchPTInventoryItemInstance* CreatedInstance = AddPotionToInventory(CraftingResult, Target);
        bPotionCreatedSuccessfully = (CreatedInstance != nullptr);
    }
    else
    {
        // Spawn in world (as ItemDefinition)
        FVector SpawnLoc = (WorldSpawnLocation != FVector::ZeroVector) ? WorldSpawnLocation : GetOwner()->GetActorLocation();
        bPotionCreatedSuccessfully = SpawnPotionInWorld(CraftingResult, SpawnLoc);
    }

    if (!bPotionCreatedSuccessfully)
    {
        UE_LOG(LogTemp, Error, TEXT("UCauldronCraftComponent::CraftPotion: Failed to create/spawn potion"));
        return EmptyResult;
    }

    // Step 3: Store the result and clear ingredients
    CraftedPotion = CraftingResult;
    ClearIngredientSlots();

    UE_LOG(LogTemp, Log, TEXT("UCauldronCraftComponent::CraftPotion: Successfully crafted potion - %s with potency %.2f"), 
           *CraftingResult.PotionEssenceTag.ToString(), 
           CraftingResult.FinalPotency);

    return CraftingResult;
}

// ----------------------------------- CRAFTING HELPER FUNCTIONS ---------------------------------------------- //

bool UCauldronCraftComponent::CanPerformCrafting() const
{
    // Check basic crafting requirements
    if (!CanCraftPotion())
    {
        UE_LOG(LogTemp, Warning, TEXT("UCauldronCraftComponent::CanPerformCrafting: Basic crafting requirements not met"));
        return false;
    }

    // Check if we have a base potion definition set
    TSubclassOf<UWitchPTInventoryItemDefinition> BasePotionItemDefinition = GetBasePotionItemDefinition();
    if (!BasePotionItemDefinition)
    {
        UE_LOG(LogTemp, Error, TEXT("UCauldronCraftComponent::CanPerformCrafting: BasePotionItemDefinition is not set"));
        return false;
    }

    // Additional checks could be added here:
    // - Player permissions
    // - Cauldron state
    // - Resource requirements
    // - Cooldowns

    return true;
}

void UCauldronCraftComponent::ClearIngredientSlots()
{
    if (!GetOwner()->HasAuthority())
    {
        UE_LOG(LogTemp, Warning, TEXT("UCauldronCraftComponent::ClearIngredientSlots: Not authority"));
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("UCauldronCraftComponent::ClearIngredientSlots: Clearing all ingredient slots"));

    // Clear base ingredient
    if (BaseIngredient)
    {
        BaseIngredient = nullptr;
        BaseIngredientIcon = nullptr;
        BroadcastBaseIngredientIconSet();
    }

    // Clear principal ingredient
    if (PrincipalIngredient)
    {
        PrincipalIngredient = nullptr;
        PrincipalIngredientIcon = nullptr;
        BroadcastPrincipalIngredientIconSet();
    }

    // Clear modifier ingredient
    if (ModifierIngredient)
    {
        ModifierIngredient = nullptr;
        ModifierIngredientIcon = nullptr;
        BroadcastModifierIngredientIconSet();
    }

    // Clear crafted potion result
    CraftedPotion = FPotionResult();

    UE_LOG(LogTemp, Log, TEXT("UCauldronCraftComponent::ClearIngredientSlots: All ingredient slots cleared"));
}

// ----------------------------------- DEBUG/TESTING HELPER FUNCTIONS ---------------------------------------------- //

void UCauldronCraftComponent::DebugTestCrafting()
{
    UE_LOG(LogTemp, Warning, TEXT("=== CAULDRON DEBUG TEST CRAFTING ==="));
    
    // Print current state
    PrintIngredientDebugData();
    
    // Test crafting calculations
    UE_LOG(LogTemp, Warning, TEXT("--- TESTING CRAFTING CALCULATIONS ---"));
    FPotionResult TestResult = CalculatePotionProperties();
    
    if (TestResult.bIsValid)
    {
        UE_LOG(LogTemp, Warning, TEXT("CALCULATION SUCCESS: %s Potion - Potency: %.2f, Duration: %.2f, Charges: %d"), 
               *TestResult.PotionEssenceTag.ToString(), 
               TestResult.FinalPotency, 
               TestResult.FinalDuration, 
               TestResult.FinalCharges);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("CALCULATION FAILED: Invalid potion result"));
    }
    
    // Test crafting validation
    UE_LOG(LogTemp, Warning, TEXT("--- TESTING CRAFTING VALIDATION ---"));
    bool bCanCraft = CanCraftPotion();
    bool bCanPerform = CanPerformCrafting();
    
    UE_LOG(LogTemp, Warning, TEXT("Can Craft Potion: %s"), bCanCraft ? TEXT("YES") : TEXT("NO"));
    UE_LOG(LogTemp, Warning, TEXT("Can Perform Crafting: %s"), bCanPerform ? TEXT("YES") : TEXT("NO"));
    
    if (bCanPerform)
    {
        UE_LOG(LogTemp, Warning, TEXT("--- ATTEMPTING WORLD SPAWN CRAFT ---"));
        FVector SpawnLocation = GetOwner()->GetActorLocation() + FVector(100, 0, 100);
        FPotionResult CraftResult = CraftPotion(false, nullptr, SpawnLocation);
        
        if (CraftResult.bIsValid)
        {
            UE_LOG(LogTemp, Warning, TEXT("CRAFTING SUCCESS: Potion definition created and ready for world spawn at %s"), *SpawnLocation.ToString());
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("CRAFTING FAILED: No valid result"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("CRAFTING VALIDATION FAILED: Cannot perform crafting"));
    }
    
    UE_LOG(LogTemp, Warning, TEXT("=== END DEBUG TEST ==="));
}

// ----------------------------------- HELPER FUNCTIONS ---------------------------------------------- //

TSubclassOf<UWitchPTInventoryItemDefinition> UCauldronCraftComponent::GetBasePotionItemDefinition() const
{
    // Get the owning cauldron actor
    ACauldronAltar* CauldronOwner = Cast<ACauldronAltar>(GetOwner());
    if (!CauldronOwner)
    {
        UE_LOG(LogTemp, Error, TEXT("UCauldronCraftComponent::GetBasePotionItemDefinition: Owner is not a CauldronAltar"));
        return nullptr;
    }

    return CauldronOwner->BasePotionItemDefinition;
}


