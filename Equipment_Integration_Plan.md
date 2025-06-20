# Equipment System - Minimal Iterative Implementation

## Goal: Start Small, Build Up
Get the simplest possible equipment system working, then add features one by one.

---

# Phase 1: Minimal Equipment Classes

## Step 1: Equipment Definition (5 minutes)

**File**: `Source/WitchPT/Public/Equipment/WitchPTEquipmentDefinition.h`
```cpp
#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WitchPTEquipmentDefinition.generated.h"

class UWitchPTEquipmentInstance;

UCLASS(Blueprintable, BlueprintType)
class WITCHPT_API UWitchPTEquipmentDefinition : public UObject
{
    GENERATED_BODY()
public:
    UWitchPTEquipmentDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
    
    UPROPERTY(EditDefaultsOnly, Category=Equipment)
    TSubclassOf<UWitchPTEquipmentInstance> InstanceType;
};
```

**File**: `Source/WitchPT/Private/Equipment/WitchPTEquipmentDefinition.cpp`
```cpp
#include "Equipment/WitchPTEquipmentDefinition.h"
#include "Equipment/WitchPTEquipmentInstance.h"

UWitchPTEquipmentDefinition::UWitchPTEquipmentDefinition(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    InstanceType = UWitchPTEquipmentInstance::StaticClass();
}
```

## Step 2: Equipment Instance (5 minutes)

**File**: `Source/WitchPT/Public/Equipment/WitchPTEquipmentInstance.h`
```cpp
#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WitchPTEquipmentInstance.generated.h"

UCLASS(BlueprintType)
class WITCHPT_API UWitchPTEquipmentInstance : public UObject
{
    GENERATED_BODY()
public:
    UWitchPTEquipmentInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
    
    virtual void OnEquipped();
    virtual void OnUnequipped();
    
    UFUNCTION(BlueprintPure, Category=Equipment)
    APawn* GetPawn() const;

protected:
    UFUNCTION(BlueprintImplementableEvent, Category=Equipment)
    void K2_OnEquipped();
    
    UFUNCTION(BlueprintImplementableEvent, Category=Equipment)
    void K2_OnUnequipped();
};
```

**File**: `Source/WitchPT/Private/Equipment/WitchPTEquipmentInstance.cpp`
```cpp
#include "Equipment/WitchPTEquipmentInstance.h"
#include "GameFramework/Pawn.h"

UWitchPTEquipmentInstance::UWitchPTEquipmentInstance(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

APawn* UWitchPTEquipmentInstance::GetPawn() const
{
    return Cast<APawn>(GetOuter());
}

void UWitchPTEquipmentInstance::OnEquipped()
{
    UE_LOG(LogTemp, Warning, TEXT("✅ EQUIPPED: %s"), *GetClass()->GetName());
    K2_OnEquipped();
}

void UWitchPTEquipmentInstance::OnUnequipped()
{
    UE_LOG(LogTemp, Warning, TEXT("❌ UNEQUIPPED: %s"), *GetClass()->GetName());
    K2_OnUnequipped();
}
```

## Step 3: Equipment Manager (10 minutes)

**File**: `Source/WitchPT/Public/Equipment/WitchPTEquipmentManagerComponent.h`
```cpp
#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WitchPTEquipmentManagerComponent.generated.h"

class UWitchPTEquipmentDefinition;
class UWitchPTEquipmentInstance;

UCLASS(BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class WITCHPT_API UWitchPTEquipmentManagerComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    UWitchPTEquipmentManagerComponent();
    
    UFUNCTION(BlueprintCallable, Category="Equipment")
    UWitchPTEquipmentInstance* EquipItem(TSubclassOf<UWitchPTEquipmentDefinition> EquipmentDefinition);
    
    UFUNCTION(BlueprintCallable, Category="Equipment")
    void UnequipItem(UWitchPTEquipmentInstance* ItemInstance);
    
    UFUNCTION(BlueprintCallable, Category="Equipment")
    void PrintEquippedItems();

protected:
    UPROPERTY(VisibleAnywhere, Category="Equipment")
    TArray<TObjectPtr<UWitchPTEquipmentInstance>> EquippedItems;
};
```

**File**: `Source/WitchPT/Private/Equipment/WitchPTEquipmentManagerComponent.cpp`
```cpp
#include "Equipment/WitchPTEquipmentManagerComponent.h"
#include "Equipment/WitchPTEquipmentDefinition.h"
#include "Equipment/WitchPTEquipmentInstance.h"

UWitchPTEquipmentManagerComponent::UWitchPTEquipmentManagerComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

UWitchPTEquipmentInstance* UWitchPTEquipmentManagerComponent::EquipItem(TSubclassOf<UWitchPTEquipmentDefinition> EquipmentDefinition)
{
    if (!EquipmentDefinition) return nullptr;
    
    APawn* OwnerPawn = Cast<APawn>(GetOwner());
    if (!OwnerPawn) return nullptr;
    
    const UWitchPTEquipmentDefinition* EquipmentCDO = GetDefault<UWitchPTEquipmentDefinition>(EquipmentDefinition);
    TSubclassOf<UWitchPTEquipmentInstance> InstanceType = EquipmentCDO->InstanceType;
    if (!InstanceType) InstanceType = UWitchPTEquipmentInstance::StaticClass();
    
    UWitchPTEquipmentInstance* NewInstance = NewObject<UWitchPTEquipmentInstance>(OwnerPawn, InstanceType);
    EquippedItems.Add(NewInstance);
    NewInstance->OnEquipped();
    
    return NewInstance;
}

void UWitchPTEquipmentManagerComponent::UnequipItem(UWitchPTEquipmentInstance* ItemInstance)
{
    if (!ItemInstance) return;
    
    ItemInstance->OnUnequipped();
    EquippedItems.Remove(ItemInstance);
}

void UWitchPTEquipmentManagerComponent::PrintEquippedItems()
{
    UE_LOG(LogTemp, Warning, TEXT("📦 EQUIPPED ITEMS: %d total"), EquippedItems.Num());
    for (int32 i = 0; i < EquippedItems.Num(); i++)
    {
        UE_LOG(LogTemp, Warning, TEXT("  [%d]: %s"), i, EquippedItems[i] ? *EquippedItems[i]->GetClass()->GetName() : TEXT("NULL"));
    }
}
```

---

# Phase 2: Test the System (10 minutes)

## Step 4: Add to Character

In your character constructor:
```cpp
EquipmentManagerComponent = CreateDefaultSubobject<UWitchPTEquipmentManagerComponent>(TEXT("EquipmentManager"));
```

## Step 5: Create Test Equipment Blueprint

1. Create Blueprint based on `UWitchPTEquipmentDefinition`
2. Name it `BP_TestEquipment`
3. Save it

## Step 6: Test Function

Add anywhere accessible (character, game mode, etc.):
```cpp
UFUNCTION(BlueprintCallable, Category="Testing")
void TestEquipment()
{
    if (UWitchPTEquipmentManagerComponent* EquipMgr = FindComponentByClass<UWitchPTEquipmentManagerComponent>())
    {
        // Replace with your blueprint path
        UClass* TestEquip = LoadClass<UWitchPTEquipmentDefinition>(nullptr, TEXT("/Game/YourPath/BP_TestEquipment"));
        if (TestEquip)
        {
            UWitchPTEquipmentInstance* Item = EquipMgr->EquipItem(TestEquip);
            EquipMgr->PrintEquippedItems();
            
            // Auto-unequip after 3 seconds
            FTimerHandle Timer;
            GetWorld()->GetTimerManager().SetTimer(Timer, [EquipMgr, Item]() {
                EquipMgr->UnequipItem(Item);
                EquipMgr->PrintEquippedItems();
            }, 3.0f, false);
        }
    }
}
```

## Expected Output
```
✅ EQUIPPED: WitchPTEquipmentInstance
📦 EQUIPPED ITEMS: 1 total
  [0]: WitchPTEquipmentInstance
// 3 seconds later...
❌ UNEQUIPPED: WitchPTEquipmentInstance  
📦 EQUIPPED ITEMS: 0 total
```

---

# Phase 3: Add Blueprint Events (15 minutes)

## Goal: Test Blueprint integration and add visual feedback

### Step 7: Create Custom Equipment Instance Blueprint
*Create Blueprint to test that C++ events trigger Blueprint logic*

1. Create Blueprint based on `UWitchPTEquipmentInstance`
2. Name it `BP_TestEquipmentInstance`
3. Add Blueprint logic:
   - **Event OnEquipped**: Print "🔥 Blueprint: Equipment Equipped!"
   - **Event OnUnequipped**: Print "❄️ Blueprint: Equipment Unequipped!"

### Step 8: Update Equipment Definition
*Tell equipment definition to use our custom Blueprint instance instead of base C++ class*

Modify your `BP_TestEquipment`:
- Set **Instance Type** to `BP_TestEquipmentInstance`

### Step 9: Test Blueprint Events
*Verify that C++ equip/unequip calls now trigger Blueprint events*

Run your test function again. You should now see:
```
✅ EQUIPPED: BP_TestEquipmentInstance_C
🔥 Blueprint: Equipment Equipped!
📦 EQUIPPED ITEMS: 1 total
  [0]: BP_TestEquipmentInstance_C
// 3 seconds later...
❌ UNEQUIPPED: BP_TestEquipmentInstance_C
❄️ Blueprint: Equipment Unequipped!
📦 EQUIPPED ITEMS: 0 total
```

### Step 10: Add Visual Feedback (Optional)
*Add visual effects to prove equipment events work in Blueprint*

In your `BP_TestEquipmentInstance`:
- **OnEquipped**: Spawn particle effect on character
- **OnUnequipped**: Stop particle effect
- Test with simple effects to verify it works

---

# Phase 4: Add Simple Abilities (20 minutes)

## Goal: Grant/remove gameplay abilities when equipment is equipped

### Step 11: Add Abilities to Equipment Definition
*Add array to store which abilities this equipment should grant when equipped*

**Update**: `Source/WitchPT/Public/Equipment/WitchPTEquipmentDefinition.h`
```cpp
// Add after InstanceType:
UPROPERTY(EditDefaultsOnly, Category=Equipment)
TArray<TSubclassOf<UGameplayAbility>> AbilitiesToGrant;
```

### Step 12: Add Ability Granting to Equipment Manager
*Add data structures to track which abilities each equipment granted so we can remove them later*

**Update**: `Source/WitchPT/Public/Equipment/WitchPTEquipmentManagerComponent.h`
```cpp
// Add includes at top:
#include "GameplayAbilitySpecHandle.h"

// Add after existing includes:
class UGameplayAbility;
class UAbilitySystemComponent;

// Add to private section:
struct FEquipmentAbilityHandles
{
    TArray<FGameplayAbilitySpecHandle> GrantedHandles;
};

// Add after EquippedItems:
UPROPERTY()
TMap<TObjectPtr<UWitchPTEquipmentInstance>, FEquipmentAbilityHandles> EquipmentAbilities;

// Add helper function:
private:
    UAbilitySystemComponent* GetAbilitySystemComponent() const;
```

**Update**: `Source/WitchPT/Private/Equipment/WitchPTEquipmentManagerComponent.cpp`
*Implement the actual ability granting/removal logic*
```cpp
// Add includes:
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameplayAbilities/Public/GameplayAbility.h"

// Add helper function - finds the character's ability system:
UAbilitySystemComponent* UWitchPTEquipmentManagerComponent::GetAbilitySystemComponent() const
{
    if (APawn* OwnerPawn = Cast<APawn>(GetOwner()))
    {
        return UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwnerPawn);
    }
    return nullptr;
}

// Update EquipItem function - add after NewInstance->OnEquipped():
// Grant abilities - give each ability in the equipment definition to the character
if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
{
    FEquipmentAbilityHandles& AbilityHandles = EquipmentAbilities.Add(NewInstance);
    for (TSubclassOf<UGameplayAbility> AbilityClass : EquipmentCDO->AbilitiesToGrant)
    {
        if (AbilityClass)
        {
            FGameplayAbilitySpec AbilitySpec(AbilityClass, 1, INDEX_NONE, NewInstance);
            FGameplayAbilitySpecHandle Handle = ASC->GiveAbility(AbilitySpec);
            AbilityHandles.GrantedHandles.Add(Handle);
            UE_LOG(LogTemp, Warning, TEXT("🎯 Granted ability: %s"), *AbilityClass->GetName());
        }
    }
}

// Update UnequipItem function - add before ItemInstance->OnUnequipped():
// Remove abilities - take back all abilities this equipment granted
if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
{
    if (FEquipmentAbilityHandles* AbilityHandles = EquipmentAbilities.Find(ItemInstance))
    {
        for (const FGameplayAbilitySpecHandle& Handle : AbilityHandles->GrantedHandles)
        {
            if (Handle.IsValid())
            {
                ASC->ClearAbility(Handle);
                UE_LOG(LogTemp, Warning, TEXT("🚫 Removed ability"));
            }
        }
        EquipmentAbilities.Remove(ItemInstance);
    }
}
```

### Step 13: Create Test Ability
*Create a simple ability to test that equipment can grant abilities to the character*

1. Create Blueprint based on `UGameplayAbility`
2. Name it `BP_TestEquipmentAbility`
3. Add simple logic (print message when activated)
4. Set input binding if desired

### Step 14: Update Equipment Definition
*Tell the equipment definition to grant our test ability when equipped*

In your `BP_TestEquipment`:
- Add `BP_TestEquipmentAbility` to **Abilities To Grant** array

### Step 15: Test Ability Granting
*Verify that equipping grants abilities and unequipping removes them*

Run test function. You should see:
```
✅ EQUIPPED: BP_TestEquipmentInstance_C
🎯 Granted ability: BP_TestEquipmentAbility_C
📦 EQUIPPED ITEMS: 1 total
// 3 seconds later...
🚫 Removed ability
❌ UNEQUIPPED: BP_TestEquipmentInstance_C
```

---

# Phase 5: Inventory Integration (25 minutes)

## Goal: Connect equipment system to existing inventory

### Step 16: Create Equipment Fragment
*Create inventory fragment that marks items as equippable and stores their equipment definition*

**File**: `Source/WitchPT/Public/Inventory/Fragments/WitchPTInventoryItemFragment_EquippableItem.h`
```cpp
#pragma once
#include "CoreMinimal.h"
#include "Inventory/WitchPTInventoryItemFragment.h"
#include "WitchPTInventoryItemFragment_EquippableItem.generated.h"

class UWitchPTEquipmentDefinition;

UCLASS()
class WITCHPT_API UWitchPTInventoryItemFragment_EquippableItem : public UWitchPTInventoryItemFragment
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Category=Equipment)
    TSubclassOf<UWitchPTEquipmentDefinition> EquipmentDefinition;
};
```

**File**: `Source/WitchPT/Private/Inventory/Fragments/WitchPTInventoryItemFragment_EquippableItem.cpp`
```cpp
#include "Inventory/Fragments/WitchPTInventoryItemFragment_EquippableItem.h"
```

### Step 17: Add Inventory Integration to Equipment Manager

**Update**: `Source/WitchPT/Public/Equipment/WitchPTEquipmentManagerComponent.h`
```cpp
// Add include:
class UWitchPTInventoryItemInstance;

// Add functions:
UFUNCTION(BlueprintCallable, Category="Equipment")
UWitchPTEquipmentInstance* EquipInventoryItem(UWitchPTInventoryItemInstance* InventoryItem);

UFUNCTION(BlueprintCallable, Category="Equipment")
void UnequipInventoryItem(UWitchPTInventoryItemInstance* InventoryItem);

UFUNCTION(BlueprintPure, Category="Equipment")
bool IsInventoryItemEquipped(UWitchPTInventoryItemInstance* InventoryItem) const;

UFUNCTION(BlueprintPure, Category="Equipment")
UWitchPTEquipmentInstance* FindEquipmentByInventoryItem(UWitchPTInventoryItemInstance* InventoryItem) const;
```

**Update**: `Source/WitchPT/Private/Equipment/WitchPTEquipmentManagerComponent.cpp`
```cpp
// Add includes:
#include "Inventory/WitchPTInventoryItemInstance.h"
#include "Inventory/Fragments/WitchPTInventoryItemFragment_EquippableItem.h"

// Add functions:
UWitchPTEquipmentInstance* UWitchPTEquipmentManagerComponent::EquipInventoryItem(UWitchPTInventoryItemInstance* InventoryItem)
{
    if (!InventoryItem) return nullptr;
    
    // Check if already equipped
    if (IsInventoryItemEquipped(InventoryItem)) return nullptr;
    
    // Find equipment fragment
    const UWitchPTInventoryItemFragment_EquippableItem* EquipFragment = 
        InventoryItem->FindFragmentByClass<UWitchPTInventoryItemFragment_EquippableItem>();
    
    if (!EquipFragment || !EquipFragment->EquipmentDefinition) return nullptr;
    
    // Equip the item
    UWitchPTEquipmentInstance* EquipmentInstance = EquipItem(EquipFragment->EquipmentDefinition);
    if (EquipmentInstance)
    {
        // Store reference to inventory item (add this to equipment instance)
        // For now, just log
        UE_LOG(LogTemp, Warning, TEXT("🎒 Equipped from inventory: %s"), *InventoryItem->GetItemDefinition()->GetName());
    }
    
    return EquipmentInstance;
}

void UWitchPTEquipmentManagerComponent::UnequipInventoryItem(UWitchPTInventoryItemInstance* InventoryItem)
{
    if (UWitchPTEquipmentInstance* Equipment = FindEquipmentByInventoryItem(InventoryItem))
    {
        UnequipItem(Equipment);
    }
}

bool UWitchPTEquipmentManagerComponent::IsInventoryItemEquipped(UWitchPTInventoryItemInstance* InventoryItem) const
{
    return FindEquipmentByInventoryItem(InventoryItem) != nullptr;
}

UWitchPTEquipmentInstance* UWitchPTEquipmentManagerComponent::FindEquipmentByInventoryItem(UWitchPTInventoryItemInstance* InventoryItem) const
{
    // For now, simple implementation - in Phase 6 we'll add proper tracking
    // This is just a placeholder that always returns nullptr
    return nullptr;
}
```

### Step 18: Add Instigator Tracking to Equipment Instance

**Update**: `Source/WitchPT/Public/Equipment/WitchPTEquipmentInstance.h`
```cpp
// Add functions:
UFUNCTION(BlueprintPure, Category=Equipment)
UObject* GetInstigator() const { return Instigator; }

void SetInstigator(UObject* InInstigator) { Instigator = InInstigator; }

// Add private member:
private:
    UPROPERTY()
    TObjectPtr<UObject> Instigator;
```

**Update**: Equipment Manager's EquipInventoryItem to set instigator:
```cpp
// After EquipmentInstance creation:
if (EquipmentInstance)
{
    EquipmentInstance->SetInstigator(InventoryItem);
    UE_LOG(LogTemp, Warning, TEXT("🎒 Equipped from inventory: %s"), *InventoryItem->GetItemDefinition()->GetName());
}
```

**Update**: FindEquipmentByInventoryItem implementation:
```cpp
UWitchPTEquipmentInstance* UWitchPTEquipmentManagerComponent::FindEquipmentByInventoryItem(UWitchPTInventoryItemInstance* InventoryItem) const
{
    for (UWitchPTEquipmentInstance* Equipment : EquippedItems)
    {
        if (Equipment && Equipment->GetInstigator() == InventoryItem)
        {
            return Equipment;
        }
    }
    return nullptr;
}
```

### Step 19: Create Test Inventory Item

1. Create `UWitchPTInventoryItemDefinition` Blueprint
2. Name it `BP_TestEquippableItem`
3. Add `UWitchPTInventoryItemFragment_EquippableItem` fragment
4. Set fragment's **Equipment Definition** to your `BP_TestEquipment`

### Step 20: Test Inventory Integration

Add test function:
```cpp
UFUNCTION(BlueprintCallable, Category="Testing")
void TestInventoryEquipment()
{
    // Create test inventory item
    UClass* ItemDefClass = LoadClass<UWitchPTInventoryItemDefinition>(nullptr, TEXT("/Game/YourPath/BP_TestEquippableItem"));
    if (ItemDefClass && InventoryManager)
    {
        UWitchPTInventoryItemInstance* TestItem = InventoryManager->CreateItemInstance(ItemDefClass, 1);
        
        if (UWitchPTEquipmentManagerComponent* EquipMgr = FindComponentByClass<UWitchPTEquipmentManagerComponent>())
        {
            // Test equip from inventory
            UWitchPTEquipmentInstance* Equipment = EquipMgr->EquipInventoryItem(TestItem);
            UE_LOG(LogTemp, Warning, TEXT("📋 Is equipped: %s"), EquipMgr->IsInventoryItemEquipped(TestItem) ? TEXT("YES") : TEXT("NO"));
            
            // Test unequip
            FTimerHandle Timer;
            GetWorld()->GetTimerManager().SetTimer(Timer, [EquipMgr, TestItem]() {
                EquipMgr->UnequipInventoryItem(TestItem);
            }, 3.0f, false);
        }
    }
}
```

---

# Phase 6: Add Networking (30 minutes)

## Goal: Make equipment system work in multiplayer

## Networking Requirements
Equipment systems need replication because:
- **Equipment instances** must exist on all clients for visual effects, animations, and UI
- **Equipment manager state** needs to sync so all players see what others have equipped
- **Server authority** prevents cheating (only server can actually equip/unequip items)
- **Ability replication** ensures abilities granted by equipment work correctly across network

## Key Concepts
- **Equipment Instance Replication**: Each equipped item becomes a replicated object
- **Authority Checks**: Only server can modify equipment state (equip/unequip)
- **Client Prediction**: Clients can show immediate visual feedback while waiting for server confirmation
- **Subobject Replication**: Equipment instances are subobjects of the character, need special handling

### Step 21: Add Replication to Equipment Instance

**Update**: `Source/WitchPT/Public/Equipment/WitchPTEquipmentInstance.h`
```cpp
// Add includes:
#include "Net/UnrealNetwork.h"

// Add to class:
//~UObject interface
virtual bool IsSupportedForNetworking() const override { return true; }
virtual UWorld* GetWorld() const override final;
virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
//~End of UObject interface

// Update Instigator:
UPROPERTY(ReplicatedUsing=OnRep_Instigator)
TObjectPtr<UObject> Instigator;

// Add rep notify:
UFUNCTION()
void OnRep_Instigator();
```

**Update**: `Source/WitchPT/Private/Equipment/WitchPTEquipmentInstance.cpp`
```cpp
// Add includes:
#include "Net/UnrealNetwork.h"

// Add functions:
UWorld* UWitchPTEquipmentInstance::GetWorld() const
{
    if (APawn* OwningPawn = GetPawn())
    {
        return OwningPawn->GetWorld();
    }
    return nullptr;
}

void UWitchPTEquipmentInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ThisClass, Instigator);
}

void UWitchPTEquipmentInstance::OnRep_Instigator()
{
    // Handle instigator replication if needed
}
```

### Step 22: Add Fast Array Replication to Equipment Manager

**Update**: `Source/WitchPT/Public/Equipment/WitchPTEquipmentManagerComponent.h`
```cpp
// Add includes:
#include "Net/Serialization/FastArraySerializer.h"
#include "Components/PawnComponent.h"

// Replace UActorComponent with UPawnComponent:
class WITCHPT_API UWitchPTEquipmentManagerComponent : public UPawnComponent

// Add fast array structures:
USTRUCT(BlueprintType)
struct FWitchPTEquipmentEntry : public FFastArraySerializerItem
{
    GENERATED_BODY()

    FWitchPTEquipmentEntry() {}

    UPROPERTY()
    TSubclassOf<UWitchPTEquipmentDefinition> EquipmentDefinition;

    UPROPERTY()
    TObjectPtr<UWitchPTEquipmentInstance> Instance = nullptr;

    // Non-replicated ability handles
    FEquipmentAbilityHandles AbilityHandles;
    
    FString GetDebugString() const;
};

USTRUCT(BlueprintType)
struct FWitchPTEquipmentList : public FFastArraySerializer
{
    GENERATED_BODY()

    FWitchPTEquipmentList() : OwnerComponent(nullptr) {}
    FWitchPTEquipmentList(UActorComponent* InOwnerComponent) : OwnerComponent(InOwnerComponent) {}

    //~FFastArraySerializer contract
    void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
    void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
    void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
    //~End of FFastArraySerializer contract

    bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
    {
        return FFastArraySerializer::FastArrayDeltaSerialize<FWitchPTEquipmentEntry, FWitchPTEquipmentList>(Entries, DeltaParms, *this);
    }

    UWitchPTEquipmentInstance* AddEntry(TSubclassOf<UWitchPTEquipmentDefinition> EquipmentDefinition);
    void RemoveEntry(UWitchPTEquipmentInstance* Instance);

private:
    UAbilitySystemComponent* GetAbilitySystemComponent() const;

    UPROPERTY()
    TArray<FWitchPTEquipmentEntry> Entries;

    UPROPERTY(NotReplicated)
    TObjectPtr<UActorComponent> OwnerComponent;
    
    friend UWitchPTEquipmentManagerComponent;
};

// Add template specialization:
template<>
struct TStructOpsTypeTraits<FWitchPTEquipmentList> : public TStructOpsTypeTraitsBase2<FWitchPTEquipmentList>
{
    enum { WithNetDeltaSerializer = true };
};

// Update component constructor:
UWitchPTEquipmentManagerComponent() : EquipmentList(this) { SetIsReplicatedByDefault(true); }

// Replace EquippedItems with:
UPROPERTY(Replicated)
FWitchPTEquipmentList EquipmentList;

// Remove old EquipmentAbilities map and EquippedItems array

// Add networking functions:
virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
virtual void ReadyForReplication() override;
virtual void InitializeComponent() override;
virtual void UninitializeComponent() override;

// Add utility functions:
UFUNCTION(BlueprintCallable, BlueprintPure)
UWitchPTEquipmentInstance* GetFirstInstanceOfType(TSubclassOf<UWitchPTEquipmentInstance> InstanceType);

UFUNCTION(BlueprintCallable, BlueprintPure)
TArray<UWitchPTEquipmentInstance*> GetEquipmentInstancesOfType(TSubclassOf<UWitchPTEquipmentInstance> InstanceType) const;
```

### Step 23: Implement Complete Fast Array Logic

**Update**: `Source/WitchPT/Private/Equipment/WitchPTEquipmentManagerComponent.cpp`
```cpp
// Add includes:
#include "Engine/ActorChannel.h"
#include "AbilitySystemGlobals.h"

//////////////////////////////////////////////////////////////////////
// FWitchPTEquipmentEntry

FString FWitchPTEquipmentEntry::GetDebugString() const
{
    return FString::Printf(TEXT("%s of %s"), *GetNameSafe(Instance), *GetNameSafe(EquipmentDefinition.Get()));
}

//////////////////////////////////////////////////////////////////////
// FWitchPTEquipmentList

void FWitchPTEquipmentList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
    for (int32 Index : RemovedIndices)
    {
        const FWitchPTEquipmentEntry& Entry = Entries[Index];
        if (Entry.Instance != nullptr)
        {
            Entry.Instance->OnUnequipped();
        }
    }
}

void FWitchPTEquipmentList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
    for (int32 Index : AddedIndices)
    {
        const FWitchPTEquipmentEntry& Entry = Entries[Index];
        if (Entry.Instance != nullptr)
        {
            Entry.Instance->OnEquipped();
        }
    }
}

void FWitchPTEquipmentList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
    // Handle any changes to existing entries if needed
}

UAbilitySystemComponent* FWitchPTEquipmentList::GetAbilitySystemComponent() const
{
    check(OwnerComponent);
    AActor* OwningActor = OwnerComponent->GetOwner();
    return Cast<UAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor));
}

UWitchPTEquipmentInstance* FWitchPTEquipmentList::AddEntry(TSubclassOf<UWitchPTEquipmentDefinition> EquipmentDefinition)
{
    UWitchPTEquipmentInstance* Result = nullptr;

    check(EquipmentDefinition != nullptr);
    check(OwnerComponent);
    check(OwnerComponent->GetOwner()->HasAuthority());
    
    const UWitchPTEquipmentDefinition* EquipmentCDO = GetDefault<UWitchPTEquipmentDefinition>(EquipmentDefinition);

    TSubclassOf<UWitchPTEquipmentInstance> InstanceType = EquipmentCDO->InstanceType;
    if (InstanceType == nullptr)
    {
        InstanceType = UWitchPTEquipmentInstance::StaticClass();
    }
    
    FWitchPTEquipmentEntry& NewEntry = Entries.AddDefaulted_GetRef();
    NewEntry.EquipmentDefinition = EquipmentDefinition;
    NewEntry.Instance = NewObject<UWitchPTEquipmentInstance>(OwnerComponent->GetOwner(), InstanceType);
    Result = NewEntry.Instance;

    // Grant abilities
    if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
    {
        for (TSubclassOf<UGameplayAbility> AbilityClass : EquipmentCDO->AbilitiesToGrant)
        {
            if (AbilityClass)
            {
                FGameplayAbilitySpec AbilitySpec(AbilityClass, 1, INDEX_NONE, Result);
                FGameplayAbilitySpecHandle Handle = ASC->GiveAbility(AbilitySpec);
                NewEntry.AbilityHandles.GrantedHandles.Add(Handle);
                UE_LOG(LogTemp, Warning, TEXT("🎯 Granted ability: %s"), *AbilityClass->GetName());
            }
        }
    }

    MarkItemDirty(NewEntry);
    return Result;
}

void FWitchPTEquipmentList::RemoveEntry(UWitchPTEquipmentInstance* Instance)
{
    for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
    {
        FWitchPTEquipmentEntry& Entry = *EntryIt;
        if (Entry.Instance == Instance)
        {
            // Remove abilities
            if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
            {
                for (const FGameplayAbilitySpecHandle& Handle : Entry.AbilityHandles.GrantedHandles)
                {
                    if (Handle.IsValid())
                    {
                        ASC->ClearAbility(Handle);
                        UE_LOG(LogTemp, Warning, TEXT("🚫 Removed ability"));
                    }
                }
            }

            EntryIt.RemoveCurrent();
            MarkArrayDirty();
        }
    }
}

//////////////////////////////////////////////////////////////////////
// UWitchPTEquipmentManagerComponent

UWitchPTEquipmentManagerComponent::UWitchPTEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , EquipmentList(this)
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
    bWantsInitializeComponent = true;
}

void UWitchPTEquipmentManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ThisClass, EquipmentList);
}

UWitchPTEquipmentInstance* UWitchPTEquipmentManagerComponent::EquipItem(TSubclassOf<UWitchPTEquipmentDefinition> EquipmentDefinition)
{
    UWitchPTEquipmentInstance* Result = nullptr;
    if (EquipmentDefinition != nullptr)
    {
        if (!GetOwner()->HasAuthority()) return nullptr;
        
        Result = EquipmentList.AddEntry(EquipmentDefinition);
        if (Result != nullptr)
        {
            Result->OnEquipped();
            
            if (IsUsingRegisteredSubObjectList() && IsReadyForReplication())
            {
                AddReplicatedSubObject(Result);
            }
        }
    }
    return Result;
}

void UWitchPTEquipmentManagerComponent::UnequipItem(UWitchPTEquipmentInstance* ItemInstance)
{
    if (ItemInstance != nullptr)
    {
        if (!GetOwner()->HasAuthority()) return;
        
        if (IsUsingRegisteredSubObjectList())
        {
            RemoveReplicatedSubObject(ItemInstance);
        }

        ItemInstance->OnUnequipped();
        EquipmentList.RemoveEntry(ItemInstance);
    }
}

bool UWitchPTEquipmentManagerComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
    bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

    for (FWitchPTEquipmentEntry& Entry : EquipmentList.Entries)
    {
        UWitchPTEquipmentInstance* Instance = Entry.Instance;
        if (IsValid(Instance))
        {
            WroteSomething |= Channel->ReplicateSubobject(Instance, *Bunch, *RepFlags);
        }
    }

    return WroteSomething;
}

void UWitchPTEquipmentManagerComponent::InitializeComponent()
{
    Super::InitializeComponent();
}

void UWitchPTEquipmentManagerComponent::UninitializeComponent()
{
    TArray<UWitchPTEquipmentInstance*> AllEquipmentInstances;

    // Gather all instances before removal to avoid side effects affecting the equipment list iterator    
    for (const FWitchPTEquipmentEntry& Entry : EquipmentList.Entries)
    {
        AllEquipmentInstances.Add(Entry.Instance);
    }

    for (UWitchPTEquipmentInstance* EquipInstance : AllEquipmentInstances)
    {
        UnequipItem(EquipInstance);
    }

    Super::UninitializeComponent();
}

void UWitchPTEquipmentManagerComponent::ReadyForReplication()
{
    Super::ReadyForReplication();

    // Register existing equipment instances
    if (IsUsingRegisteredSubObjectList())
    {
        for (const FWitchPTEquipmentEntry& Entry : EquipmentList.Entries)
        {
            UWitchPTEquipmentInstance* Instance = Entry.Instance;
            if (IsValid(Instance))
            {
                AddReplicatedSubObject(Instance);
            }
        }
    }
}

UWitchPTEquipmentInstance* UWitchPTEquipmentManagerComponent::GetFirstInstanceOfType(TSubclassOf<UWitchPTEquipmentInstance> InstanceType)
{
    for (FWitchPTEquipmentEntry& Entry : EquipmentList.Entries)
    {
        if (UWitchPTEquipmentInstance* Instance = Entry.Instance)
        {
            if (Instance->IsA(InstanceType))
            {
                return Instance;
            }
        }
    }
    return nullptr;
}

TArray<UWitchPTEquipmentInstance*> UWitchPTEquipmentManagerComponent::GetEquipmentInstancesOfType(TSubclassOf<UWitchPTEquipmentInstance> InstanceType) const
{
    TArray<UWitchPTEquipmentInstance*> Results;
    for (const FWitchPTEquipmentEntry& Entry : EquipmentList.Entries)
    {
        if (UWitchPTEquipmentInstance* Instance = Entry.Instance)
        {
            if (Instance->IsA(InstanceType))
            {
                Results.Add(Instance);
            }
        }
    }
    return Results;
}

// Update inventory integration functions to use EquipmentList:
UWitchPTEquipmentInstance* UWitchPTEquipmentManagerComponent::FindEquipmentByInventoryItem(UWitchPTInventoryItemInstance* InventoryItem) const
{
    for (const FWitchPTEquipmentEntry& Entry : EquipmentList.Entries)
    {
        if (UWitchPTEquipmentInstance* Equipment = Entry.Instance)
        {
            if (Equipment && Equipment->GetInstigator() == InventoryItem)
            {
                return Equipment;
            }
        }
    }
    return nullptr;
}

void UWitchPTEquipmentManagerComponent::PrintEquippedItems()
{
    UE_LOG(LogTemp, Warning, TEXT("📦 EQUIPPED ITEMS: %d total"), EquipmentList.Entries.Num());
    for (int32 i = 0; i < EquipmentList.Entries.Num(); i++)
    {
        const FWitchPTEquipmentEntry& Entry = EquipmentList.Entries[i];
        UE_LOG(LogTemp, Warning, TEXT("  [%d]: %s"), i, Entry.Instance ? *Entry.Instance->GetClass()->GetName() : TEXT("NULL"));
    }
}
```

### Step 24: Test Networking

1. **Enable multiplayer testing** in project settings
2. **Test with 2+ players** - one server, one client
3. **Verify equipment replicates** between clients
4. **Check that abilities only work on server**

Expected behavior:
- Equipment equips/unequips on all clients
- Abilities are granted/removed properly
- Visual effects show on all clients

---

# Phase 7: Integration with Potion System (15 minutes)

## Goal: Make crafted potions equippable

### Step 25: Update Potion Creation

**Update**: Your `CauldronCraftComponent::CreatePotionItemDefinitionInstance()`:
```cpp
// After creating potion properties fragment, add:
UWitchPTInventoryItemFragment_EquippableItem* EquippableFragment = 
    NewObject<UWitchPTInventoryItemFragment_EquippableItem>(PotionDefinition);

// Set equipment definition based on potion type
EquippableFragment->EquipmentDefinition = GetPotionEquipmentDefinition(PotionData);
PotionDefinition->Fragments.Add(EquippableFragment);

// Add helper function:
TSubclassOf<UWitchPTEquipmentDefinition> GetPotionEquipmentDefinition(const FPotionResult& PotionData)
{
    // Return different equipment based on potion properties
    // For now, return a base potion equipment definition
    return BasePotionEquipmentDefinition; // Add this property to component
}
```

### Step 26: Create Potion Equipment Definition

1. Create `BP_PotionEquipmentDefinition` based on `UWitchPTEquipmentDefinition`
2. Add potion-specific abilities (drink, throw, etc.)
3. Set as `BasePotionEquipmentDefinition` in CauldronCraftComponent

### Step 27: Test Potion Equipment

1. **Craft a potion** using existing system
2. **Try to equip it** using equipment manager
3. **Verify abilities are granted**
4. **Test potion-specific functionality**

---

# Success Criteria Summary

## ✅ Phase 1-2: Basic System (DONE)
- Equipment definitions and instances created
- Equipment manager handles equip/unequip
- Basic logging and debug functions work

## ✅ Phase 3: Blueprint Events
- Custom equipment instances respond to Blueprint events
- Visual feedback when equipment is equipped/unequipped

## ✅ Phase 4: Simple Abilities
- Equipment grants gameplay abilities when equipped
- Abilities are properly removed when unequipped
- Test ability activation works

## ✅ Phase 5: Inventory Integration  
- Inventory items can be equipped through equipment manager
- Equipment tracks source inventory item
- Proper integration with existing inventory system

## ✅ Phase 6: Networking
- Equipment replicates properly in multiplayer
- Authority checks prevent client-side cheating
- All clients see equipment changes

## ✅ Phase 7: Potion Integration
- Crafted potions are automatically equippable
- Potion equipment grants appropriate abilities
- Full integration with existing potion crafting system

**Each phase builds on the previous - test thoroughly before moving to the next!** 