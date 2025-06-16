# Potion Crafting System - Testing Guide

## Overview
This guide will walk you through testing the complete potion crafting system, from setup to spawning potions in the world.

## Prerequisites Setup

### 1. Create Base Potion Item Definition (Blueprint)

1. **Create Blueprint Class**:
   - Right-click in Content Browser → Blueprint Class
   - Search for `WitchPTInventoryItemDefinition`
   - Name it `BP_BasePotionDefinition`

2. **Add Fragments to the Base Potion**:
   - Open `BP_BasePotionDefinition`
   - In the **Fragments** array, add these fragments:
     - `WitchPTInventoryItemFragment_PotionProperties` (for potion data)
     - `WitchPTInventoryFragment_UIDetails` (for icon/display)
     - `WitchPTInventoryItemFragment_WorldDetails` (for world mesh)
   - Configure the fragments:
     - **UI Details**: Set potion icon if you have one
     - **World Details**: Set `WorldStaticMesh` to a potion bottle mesh (or any mesh for testing)

### 2. Create Test Ingredient Definitions (Blueprint)

You'll need at least 2 ingredients to test basic crafting:

#### Base Ingredient Blueprint (`BP_TestBaseIngredient`):
1. Create new `WitchPTInventoryItemDefinition` Blueprint
2. Add `WitchPTInventoryItemFragment_IngredientCraftingProperties` fragment
3. Configure the fragment:
   - **SlotUsageTag**: `Item.Consumable.Ingredient.CanBeUsedIn.BaseSlot`
   - **TierTag**: `Item.Tier.Common` (or similar)
   - **CategoryTag**: `Item.Category.Ingredient.Base`
   - **BaseIngredientData**:
     - BaseLiquidTypeTag: `Item.Liquid.Water` (or similar)
     - DefaultChargesProvided: `3`
     - StabilityModifier: `1.0`

#### Principal Ingredient Blueprint (`BP_TestPrincipalIngredient`):
1. Create new `WitchPTInventoryItemDefinition` Blueprint
2. Add `WitchPTInventoryItemFragment_IngredientCraftingProperties` fragment
3. Configure the fragment:
   - **SlotUsageTag**: `Item.Consumable.Ingredient.CanBeUsedIn.PrincipalSlot`
   - **TierTag**: `Item.Tier.Common`
   - **CategoryTag**: `Item.Category.Ingredient.Principal`
   - **PrincipalIngredientData**:
     - PrincipalEssenceTag: `Effect.Healing` (or similar)
     - BasePotency: `50.0`
     - BaseDuration: `10.0`
     - bIsRitualEssence: `false`

### 3. Create Test Cauldron Actor

1. **Create Blueprint Actor** (`BP_TestCauldron`):
   - Right-click → Blueprint Class → Actor
   - Name it `BP_TestCauldron`

2. **Add Components**:
   - Add **Static Mesh Component** (for visual representation)
   - Add **CauldronCraftComponent**

3. **Configure the Cauldron**:
   - Set **BasePotionItemDefinition** to your `BP_BasePotionDefinition` (this is now on the CauldronAltar, not the component)
   - In the **CauldronCraftComponent**, configure **DefaultCraftingRecipe**:
     - bRequiresBaseIngredient: `true`
     - bRequiresPrincipalIngredient: `true`
     - bRequiresModifierIngredient: `false`

## Testing Process

### Step 1: Place Test Setup in Level

1. **Place the Cauldron**:
   - Drag `BP_TestCauldron` into your level
   - Position it where you want to test

2. **Create Test Character**:
   - Ensure you have a character with `WitchPTPlayerController`
   - The character needs `WitchPTInventoryManagerComponent`

### Step 2: Add Test Ingredients to Inventory

You have several options:

#### Option A: Add via Blueprint/C++
```cpp
// In your PlayerController or GameMode
UWitchPTInventoryManagerComponent* InventoryManager = // Get inventory manager
InventoryManager->Server_AddItemDefinition(BP_TestBaseIngredient, 5);
InventoryManager->Server_AddItemDefinition(BP_TestPrincipalIngredient, 5);
```

#### Option B: Manual Setup (Temporary)
- Create a test function that spawns ingredients directly in the cauldron slots
- Or modify the cauldron to have debug ingredients pre-placed

### Step 3: Test Ingredient Placement

1. **Call TrySetIngredientInSlot**:
   ```cpp
   // Via Blueprint or C++
   CauldronCraftComponent->TrySetIngredientInSlot(PlayerCharacter, BP_TestBaseIngredient);
   CauldronCraftComponent->TrySetIngredientInSlot(PlayerCharacter, BP_TestPrincipalIngredient);
   ```

2. **Verify Placement**:
   - Call `PrintIngredientDebugData()` to see if ingredients are properly set
   - Check the console logs for confirmation

### Step 4: Test Crafting Validation

1. **Check Crafting Status**:
   ```cpp
   bool bCanCraft = CauldronCraftComponent->CanCraftPotion();
   bool bCanPerform = CauldronCraftComponent->CanPerformCrafting();
   ```

2. **Debug Current State**:
   ```cpp
   CauldronCraftComponent->PrintIngredientDebugData();
   ```

### Step 5: Test Potion Calculation

1. **Calculate Properties**:
   ```cpp
   FPotionResult TestResult = CauldronCraftComponent->CalculatePotionProperties();
   ```

2. **Verify Results**:
   - Check console logs for calculation details
   - Verify `TestResult.bIsValid` is true
   - Check potency, duration, and charges values

### Step 6: Test World Spawning

1. **Craft Potion (World Spawn)**:
   ```cpp
   // Spawn at cauldron location
   FPotionResult Result = CauldronCraftComponent->CraftPotion(false, nullptr, FVector::ZeroVector);
   
   // Or spawn at specific location
   FVector SpawnLocation = CauldronActor->GetActorLocation() + FVector(100, 0, 50);
   FPotionResult Result = CauldronCraftComponent->CraftPotion(false, nullptr, SpawnLocation);
   ```

2. **Alternative: Use Editor Button**:
   - Select the Cauldron in the level
   - In the Details panel, find the CauldronCraftComponent
   - Look for the **"Craft Potion"** button (CallInEditor)
   - Click it to trigger crafting

## Expected Results

### Console Log Output
You should see logs like:
```
UCauldronCraftComponent::CraftPotion: Starting potion crafting process...
UCauldronCraftComponent::CalculatePotionProperties: Starting potion calculation...
UCauldronCraftComponent::CreatePotionItemDefinitionInstance: Created potion definition with properties - Effect.Healing
UCauldronCraftComponent::SpawnPotionInWorld: Applied mesh and materials from WorldDetails fragment
UCauldronCraftComponent::SpawnPotionInWorld: Successfully spawned potion at location: X=100.000000 Y=200.000000 Z=300.000000
UCauldronCraftComponent::CraftPotion: Successfully crafted potion - Effect.Healing with potency 50.00
UCauldronCraftComponent::ClearIngredientSlots: All ingredient slots cleared
```

### What Should Happen:
1. **Ingredients consumed** from inventory
2. **Cauldron slots cleared** after crafting
3. **Potion definition created** with proper properties from crafting calculations
4. **PotionBase actor spawned** in world with mesh from WorldDetails fragment
5. **Potion is interactable** and ready for pickup (handled by PotionBase)
6. **CraftedPotion result stored** in component

### Architecture Note:
- **World spawning** creates **PotionBase actors** with **ItemDefinitions**
- **Inventory** contains **ItemInstances** (created from definitions when picked up)
- **Pickup system** converts ItemDefinition → ItemInstance when items are collected
- **WorldDetails fragment** provides the visual mesh for world representation

## Debug Commands

### Blueprint/C++ Debug Functions:
```cpp
// Print all ingredient data
CauldronCraftComponent->PrintIngredientDebugData();

// Test calculations
FPotionResult TestResult = CauldronCraftComponent->CalculatePotionProperties();

// Check crafting status
bool bCanCraft = CauldronCraftComponent->CanPerformCrafting();
```

### Console Commands (if you want to add them):
```cpp
// Add to your PlayerController or GameMode
UFUNCTION(Exec)
void TestCraftPotion()
{
    // Find cauldron and trigger crafting
}

UFUNCTION(Exec) 
void DebugCauldron()
{
    // Print debug info
}
```

## Troubleshooting

### Common Issues:

1. **"BasePotionItemDefinition is not set"**:
   - Make sure you set the BasePotionItemDefinition in the CauldronAltar actor (not the component anymore)

2. **"No crafting properties fragment found"**:
   - Check that your ingredient definitions have the correct fragment
   - Verify fragment configuration (SlotUsageTag, etc.)

3. **"Cannot craft potion with current ingredients"**:
   - Verify ingredients have correct SlotUsageTag values
   - Check that required ingredients are present
   - Run `PrintIngredientDebugData()` to see what's missing

4. **"Not authority - crafting not allowed"**:
   - Make sure you're testing in a context where the actor has authority
   - In single-player, this should work fine

### Verification Checklist:
- [ ] Base potion definition created and assigned to CauldronAltar
- [ ] Test ingredients created with proper fragments
- [ ] Ingredients have correct SlotUsageTag values
- [ ] Cauldron actor and component configured properly
- [ ] Ingredients successfully added to cauldron slots
- [ ] CanPerformCrafting() returns true
- [ ] Crafting produces valid FPotionResult
- [ ] Console logs show successful crafting process

## Next Steps

Once basic crafting works:
1. Test with modifier ingredients (3-ingredient recipes)
2. Test different ingredient combinations
3. Implement actual world pickup spawning
4. Add UI integration
5. Test inventory addition (once the correct inventory function is identified)

This system provides a solid foundation for your potion crafting mechanics! 