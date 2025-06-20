# Equipment System Integration Plan

## Overview
This document outlines the plan to create a standalone equipment system that integrates with the existing `WitchPTInventoryManagerComponent`. The equipment system will be managed by a separate `UWitchPTEquipmentManagerComponent` that handles equipment instances and their lifecycle.

## Goals
- Create a standalone Lyra-style equipment system
- Manage equipment through dedicated `UWitchPTEquipmentManagerComponent`
- Support equipping items from inventory with automatic ability granting
- Maintain proper replication and networking
- Integrate with existing potion crafting system
- Use simple GameplayAbility arrays instead of ability sets

---

## Phase 1: Core Equipment Infrastructure

### Step 1: Create Equipment Definition and Instance Classes

#### Create `UWitchPTEquipmentDefinition`
**File**: `Source/WitchPT/Public/Equipment/WitchPTEquipmentDefinition.h`

```cpp
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayAbilitySpec.h"
#include "WitchPTEquipmentDefinition.generated.h"

class UWitchPTEquipmentInstance;
class UGameplayAbility;
class AActor;

USTRUCT(BlueprintType)
struct FWitchPTEquipmentActorToSpawn
{
    GENERATED_BODY()

    FWitchPTEquipmentActorToSpawn() {}

    UPROPERTY(EditAnywhere, Category=Equipment)
    TSubclassOf<AActor> ActorToSpawn;

    UPROPERTY(EditAnywhere, Category=Equipment)
    FName AttachSocket;

    UPROPERTY(EditAnywhere, Category=Equipment)
    FTransform AttachTransform;
};

/**
 * UWitchPTEquipmentDefinition
 * Definition of a piece of equipment that can be applied to a pawn
 */
UCLASS(Blueprintable, Const, Abstract, BlueprintType)
class WITCHPT_API UWitchPTEquipmentDefinition : public UObject
{
    GENERATED_BODY()

public:
    UWitchPTEquipmentDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    // Class to spawn for equipment instance
    UPROPERTY(EditDefaultsOnly, Category=Equipment)
    TSubclassOf<UWitchPTEquipmentInstance> InstanceType;

    // Gameplay abilities to grant when equipped
    UPROPERTY(EditDefaultsOnly, Category=Equipment)
    TArray<TSubclassOf<UGameplayAbility>> AbilitiesToGrant;
    
    // Actors to spawn on the pawn when equipped
    UPROPERTY(EditDefaultsOnly, Category=Equipment)
    TArray<FWitchPTEquipmentActorToSpawn> ActorsToSpawn;
};
```

#### Create `UWitchPTEquipmentInstance`
**File**: `Source/WitchPT/Public/Equipment/WitchPTEquipmentInstance.h`

```cpp
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Net/UnrealNetwork.h"
#include "WitchPTEquipmentInstance.generated.h"

class AActor;
class APawn;
struct FWitchPTEquipmentActorToSpawn;

/**
 * UWitchPTEquipmentInstance
 * A piece of equipment spawned and applied to a pawn
 */
UCLASS(BlueprintType, Blueprintable)
class WITCHPT_API UWitchPTEquipmentInstance : public UObject
{
    GENERATED_BODY()

public:
    UWitchPTEquipmentInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    //~UObject interface
    virtual bool IsSupportedForNetworking() const override { return true; }
    virtual UWorld* GetWorld() const override final;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    //~End of UObject interface

    // Equipment lifecycle
    virtual void OnEquipped();
    virtual void OnUnequipped();
    
    // Actor spawning
    virtual void SpawnEquipmentActors(const TArray<FWitchPTEquipmentActorToSpawn>& ActorsToSpawn);
    virtual void DestroyEquipmentActors();
    
    // Getters
    UFUNCTION(BlueprintPure, Category=Equipment)
    UObject* GetInstigator() const { return Instigator; }
    
    void SetInstigator(UObject* InInstigator) { Instigator = InInstigator; }
    
    UFUNCTION(BlueprintPure, Category=Equipment)
    APawn* GetPawn() const;

    UFUNCTION(BlueprintPure, Category=Equipment, meta=(DeterminesOutputType=PawnType))
    APawn* GetTypedPawn(TSubclassOf<APawn> PawnType) const;

    UFUNCTION(BlueprintPure, Category=Equipment)
    TArray<AActor*> GetSpawnedActors() const { return SpawnedActors; }

protected:
    UFUNCTION(BlueprintImplementableEvent, Category=Equipment, meta=(DisplayName="OnEquipped"))
    void K2_OnEquipped();

    UFUNCTION(BlueprintImplementableEvent, Category=Equipment, meta=(DisplayName="OnUnequipped"))
    void K2_OnUnequipped();

private:
    UFUNCTION()
    void OnRep_Instigator();

private:
    UPROPERTY(ReplicatedUsing=OnRep_Instigator)
    TObjectPtr<UObject> Instigator;
    
    UPROPERTY(Replicated)
    TArray<TObjectPtr<AActor>> SpawnedActors;
};
```

### Step 2: Create Equipment Fragment for Inventory Items

#### Create `UWitchPTInventoryItemFragment_EquippableItem`
**File**: `Source/WitchPT/Public/Inventory/Fragments/WitchPTInventoryItemFragment_EquippableItem.h`

```cpp
#pragma once

#include "CoreMinimal.h"
#include "Inventory/WitchPTInventoryItemFragment.h"
#include "WitchPTInventoryItemFragment_EquippableItem.generated.h"

class UWitchPTEquipmentDefinition;

/**
 * Fragment that makes an inventory item equippable
 */
UCLASS()
class WITCHPT_API UWitchPTInventoryItemFragment_EquippableItem : public UWitchPTInventoryItemFragment
{
    GENERATED_BODY()

public:
    // Equipment definition to create when this item is equipped
    UPROPERTY(EditAnywhere, Category=Equipment)
    TSubclassOf<UWitchPTEquipmentDefinition> EquipmentDefinition;
};
```

### Step 3: Create Equipment Manager Component

#### Create `UWitchPTEquipmentManagerComponent`
**File**: `Source/WitchPT/Public/Equipment/WitchPTEquipmentManagerComponent.h`

```cpp
#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "GameplayAbilitySpecHandle.h"
#include "WitchPTEquipmentManagerComponent.generated.h"

class UWitchPTEquipmentDefinition;
class UWitchPTEquipmentInstance;
class UWitchPTInventoryItemInstance;
class UAbilitySystemComponent;
class UGameplayAbility;

/**
 * Structure to hold granted ability handles for cleanup
 */
USTRUCT()
struct FWitchPTGrantedAbilityHandles
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;

    void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);
    void TakeFromAbilitySystem(UAbilitySystemComponent* ASC);
};

/** A single piece of applied equipment */
USTRUCT(BlueprintType)
struct FWitchPTEquipmentEntry : public FFastArraySerializerItem
{
    GENERATED_BODY()

    FWitchPTEquipmentEntry() {}

    FString GetDebugString() const;

private:
    friend struct FWitchPTEquipmentList;
    friend class UWitchPTEquipmentManagerComponent;

    // The equipment definition that was equipped
    UPROPERTY()
    TSubclassOf<UWitchPTEquipmentDefinition> EquipmentDefinition;
    
    // The equipment instance
    UPROPERTY()
    TObjectPtr<UWitchPTEquipmentInstance> Instance = nullptr;
    
    // Authority-only granted ability handles
    UPROPERTY(NotReplicated)
    FWitchPTGrantedAbilityHandles GrantedHandles;
};

/** List of applied equipment */
USTRUCT(BlueprintType)
struct FWitchPTEquipmentList : public FFastArraySerializer
{
    GENERATED_BODY()

    FWitchPTEquipmentList() : OwnerComponent(nullptr) {}
    
    FWitchPTEquipmentList(UActorComponent* InOwnerComponent) : OwnerComponent(InOwnerComponent) {}

public:
    //~FFastArraySerializer contract
    void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
    void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
    void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
    //~End of FFastArraySerializer contract

    bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
    {
        return FFastArraySerializer::FastArrayDeltaSerialize<FWitchPTEquipmentEntry, FWitchPTEquipmentList>(Entries, DeltaParms, *this);
    }
    
    // Equipment management
    UWitchPTEquipmentInstance* AddEntry(TSubclassOf<UWitchPTEquipmentDefinition> EquipmentDefinition);
    void RemoveEntry(UWitchPTEquipmentInstance* Instance);

private:
    UAbilitySystemComponent* GetAbilitySystemComponent() const;

    friend UWitchPTEquipmentManagerComponent;

private:
    // Replicated list of equipment entries
    UPROPERTY()
    TArray<FWitchPTEquipmentEntry> Entries;

    UPROPERTY(NotReplicated)
    TObjectPtr<UActorComponent> OwnerComponent;
};

template<>
struct TStructOpsTypeTraits<FWitchPTEquipmentList> : public TStructOpsTypeTraitsBase2<FWitchPTEquipmentList>
{
    enum { WithNetDeltaSerializer = true };
};

/**
 * Component that manages equipment applied to a pawn
 */
UCLASS(BlueprintType, Const)
class WITCHPT_API UWitchPTEquipmentManagerComponent : public UPawnComponent
{
    GENERATED_BODY()

public:
    UWitchPTEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    //~UObject interface
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
    //~End of UObject interface

    //~UActorComponent interface
    virtual void InitializeComponent() override;
    virtual void UninitializeComponent() override;
    virtual void ReadyForReplication() override;
    //~End of UActorComponent interface

    // Equipment management
    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Equipment")
    UWitchPTEquipmentInstance* EquipItem(TSubclassOf<UWitchPTEquipmentDefinition> EquipmentDefinition);

    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Equipment")
    void UnequipItem(UWitchPTEquipmentInstance* ItemInstance);

    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Equipment")
    UWitchPTEquipmentInstance* EquipInventoryItem(UWitchPTInventoryItemInstance* InventoryItem);

    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Equipment")
    void UnequipInventoryItem(UWitchPTInventoryItemInstance* InventoryItem);

    // Query functions
    UFUNCTION(BlueprintCallable, BlueprintPure, Category="Equipment")
    UWitchPTEquipmentInstance* GetFirstInstanceOfType(TSubclassOf<UWitchPTEquipmentInstance> InstanceType);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category="Equipment")
    TArray<UWitchPTEquipmentInstance*> GetEquipmentInstancesOfType(TSubclassOf<UWitchPTEquipmentInstance> InstanceType) const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category="Equipment")
    UWitchPTEquipmentInstance* FindEquipmentByInstigator(UObject* Instigator) const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category="Equipment")
    bool IsInventoryItemEquipped(UWitchPTInventoryItemInstance* InventoryItem) const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category="Equipment")
    TArray<UWitchPTEquipmentInstance*> GetAllEquipmentInstances() const;

    // Delegates
    DECLARE_MULTICAST_DELEGATE_TwoParams(FOnEquipmentChanged, UWitchPTEquipmentInstance*, bool /* bEquipped */);
    FOnEquipmentChanged OnEquipmentChanged;

    template <typename T>
    T* GetFirstInstanceOfType()
    {
        return (T*)GetFirstInstanceOfType(T::StaticClass());
    }

private:
    // Helper functions
    void GrantAbilitiesToASC(const TArray<TSubclassOf<UGameplayAbility>>& Abilities, 
                           UWitchPTEquipmentInstance* SourceObject,
                           FWitchPTGrantedAbilityHandles& OutGrantedHandles);

private:
    UPROPERTY(Replicated)
    FWitchPTEquipmentList EquipmentList;
};
```

---

## Phase 2: Implementation of Core Equipment Logic

### Step 4: Implement Equipment Definition

#### `UWitchPTEquipmentDefinition.cpp`
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

### Step 5: Implement Equipment Instance

#### `UWitchPTEquipmentInstance.cpp`
**File**: `Source/WitchPT/Private/Equipment/WitchPTEquipmentInstance.cpp`

```cpp
#include "Equipment/WitchPTEquipmentInstance.h"
#include "Equipment/WitchPTEquipmentDefinition.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

UWitchPTEquipmentInstance::UWitchPTEquipmentInstance(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

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
    DOREPLIFETIME(ThisClass, SpawnedActors);
}

APawn* UWitchPTEquipmentInstance::GetPawn() const
{
    return Cast<APawn>(GetOuter());
}

APawn* UWitchPTEquipmentInstance::GetTypedPawn(TSubclassOf<APawn> PawnType) const
{
    APawn* Result = nullptr;
    if (UClass* ActualPawnType = PawnType)
    {
        if (GetOuter()->IsA(ActualPawnType))
        {
            Result = Cast<APawn>(GetOuter());
        }
    }
    return Result;
}

void UWitchPTEquipmentInstance::SpawnEquipmentActors(const TArray<FWitchPTEquipmentActorToSpawn>& ActorsToSpawn)
{
    if (APawn* OwningPawn = GetPawn())
    {
        USceneComponent* AttachTarget = OwningPawn->GetRootComponent();
        if (ACharacter* Char = Cast<ACharacter>(OwningPawn))
        {
            AttachTarget = Char->GetMesh();
        }

        for (const FWitchPTEquipmentActorToSpawn& SpawnInfo : ActorsToSpawn)
        {
            AActor* NewActor = GetWorld()->SpawnActorDeferred<AActor>(SpawnInfo.ActorToSpawn, FTransform::Identity, OwningPawn);
            NewActor->FinishSpawning(FTransform::Identity, /*bIsDefaultTransform=*/ true);
            NewActor->SetActorRelativeTransform(SpawnInfo.AttachTransform);
            NewActor->AttachToComponent(AttachTarget, FAttachmentTransformRules::KeepRelativeTransform, SpawnInfo.AttachSocket);

            SpawnedActors.Add(NewActor);
        }
    }
}

void UWitchPTEquipmentInstance::DestroyEquipmentActors()
{
    for (AActor* Actor : SpawnedActors)
    {
        if (Actor)
        {
            Actor->Destroy();
        }
    }
    SpawnedActors.Empty();
}

void UWitchPTEquipmentInstance::OnEquipped()
{
    K2_OnEquipped();
}

void UWitchPTEquipmentInstance::OnUnequipped()
{
    K2_OnUnequipped();
}

void UWitchPTEquipmentInstance::OnRep_Instigator()
{
}
```

### Step 6: Implement Equipment Manager Component

#### Key Functions in `UWitchPTEquipmentManagerComponent.cpp`

```cpp
#include "Equipment/WitchPTEquipmentManagerComponent.h"
#include "Equipment/WitchPTEquipmentDefinition.h"
#include "Equipment/WitchPTEquipmentInstance.h"
#include "Inventory/WitchPTInventoryItemInstance.h"
#include "Inventory/Fragments/WitchPTInventoryItemFragment_EquippableItem.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameplayAbilities/Public/GameplayAbility.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"

// Constructor and basic setup
UWitchPTEquipmentManagerComponent::UWitchPTEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , EquipmentList(this)
{
    SetIsReplicatedByDefault(true);
    bWantsInitializeComponent = true;
}

// Core equip function
UWitchPTEquipmentInstance* UWitchPTEquipmentManagerComponent::EquipItem(TSubclassOf<UWitchPTEquipmentDefinition> EquipmentDefinition)
{
    if (!GetOwner()->HasAuthority())
    {
        return nullptr;
    }

    if (!EquipmentDefinition)
    {
        return nullptr;
    }

    UWitchPTEquipmentInstance* Result = EquipmentList.AddEntry(EquipmentDefinition);
    if (Result)
    {
        Result->OnEquipped();
        
        if (IsUsingRegisteredSubObjectList() && IsReadyForReplication())
        {
            AddReplicatedSubObject(Result);
        }

        OnEquipmentChanged.Broadcast(Result, true);
    }

    return Result;
}

// Equipment from inventory item
UWitchPTEquipmentInstance* UWitchPTEquipmentManagerComponent::EquipInventoryItem(UWitchPTInventoryItemInstance* InventoryItem)
{
    if (!InventoryItem)
    {
        return nullptr;
    }

    const UWitchPTInventoryItemFragment_EquippableItem* EquippableFragment = 
        InventoryItem->FindFragmentByClass<UWitchPTInventoryItemFragment_EquippableItem>();

    if (!EquippableFragment || !EquippableFragment->EquipmentDefinition)
    {
        return nullptr;
    }

    // Check if already equipped
    if (IsInventoryItemEquipped(InventoryItem))
    {
        return nullptr;
    }

    UWitchPTEquipmentInstance* EquipmentInstance = EquipItem(EquippableFragment->EquipmentDefinition);
    if (EquipmentInstance)
    {
        EquipmentInstance->SetInstigator(InventoryItem);
    }

    return EquipmentInstance;
}

// Ability granting helper
void UWitchPTEquipmentManagerComponent::GrantAbilitiesToASC(const TArray<TSubclassOf<UGameplayAbility>>& Abilities, 
                                                           UWitchPTEquipmentInstance* SourceObject,
                                                           FWitchPTGrantedAbilityHandles& OutGrantedHandles)
{
    if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner()))
    {
        for (const TSubclassOf<UGameplayAbility>& AbilityClass : Abilities)
        {
            if (AbilityClass)
            {
                FGameplayAbilitySpec AbilitySpec(AbilityClass, 1, INDEX_NONE, SourceObject);
                FGameplayAbilitySpecHandle SpecHandle = ASC->GiveAbility(AbilitySpec);
                OutGrantedHandles.AddAbilitySpecHandle(SpecHandle);
            }
        }
    }
}
```

---

## Phase 3: Equipment List Implementation

### Step 7: Implement Equipment List Logic

#### Key Equipment List Functions

```cpp
// Equipment list entry addition
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
    if (UWitchPTEquipmentManagerComponent* EquipmentManager = Cast<UWitchPTEquipmentManagerComponent>(OwnerComponent))
    {
        EquipmentManager->GrantAbilitiesToASC(EquipmentCDO->AbilitiesToGrant, Result, NewEntry.GrantedHandles);
    }

    // Spawn equipment actors
    Result->SpawnEquipmentActors(EquipmentCDO->ActorsToSpawn);

    MarkItemDirty(NewEntry);

    return Result;
}

// Equipment list entry removal
void FWitchPTEquipmentList::RemoveEntry(UWitchPTEquipmentInstance* Instance)
{
    for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
    {
        FWitchPTEquipmentEntry& Entry = *EntryIt;
        if (Entry.Instance == Instance)
        {
            if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
            {
                Entry.GrantedHandles.TakeFromAbilitySystem(ASC);
            }

            Instance->DestroyEquipmentActors();
            
            EntryIt.RemoveCurrent();
            MarkArrayDirty();
        }
    }
}
```

### Step 8: Implement Granted Ability Handles

#### Ability Handle Management

```cpp
void FWitchPTGrantedAbilityHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
    if (Handle.IsValid())
    {
        AbilitySpecHandles.Add(Handle);
    }
}

void FWitchPTGrantedAbilityHandles::TakeFromAbilitySystem(UAbilitySystemComponent* ASC)
{
    if (!ASC)
    {
        return;
    }

    for (const FGameplayAbilitySpecHandle& Handle : AbilitySpecHandles)
    {
        if (Handle.IsValid())
        {
            ASC->ClearAbility(Handle);
        }
    }

    AbilitySpecHandles.Empty();
}
```

---

## Phase 4: Integration with Existing Systems

### Step 9: Integrate with Potion System

#### Update Potion Item Definitions

Modify `CauldronCraftComponent::CreatePotionItemDefinitionInstance()`:

```cpp
UWitchPTInventoryItemDefinition* UCauldronCraftComponent::CreatePotionItemDefinitionInstance(const FPotionResult& PotionData)
{
    // ... existing potion creation logic ...

    // Add equippable fragment to crafted potions
    UWitchPTInventoryItemFragment_EquippableItem* EquippableFragment = 
        NewObject<UWitchPTInventoryItemFragment_EquippableItem>(PotionDefinition);
    
    // Set equipment definition based on potion type
    EquippableFragment->EquipmentDefinition = GetPotionEquipmentDefinitionClass(PotionData);
    
    PotionDefinition->Fragments.Add(EquippableFragment);

    return PotionDefinition;
}

// Helper function to determine equipment definition for potions
TSubclassOf<UWitchPTEquipmentDefinition> UCauldronCraftComponent::GetPotionEquipmentDefinitionClass(const FPotionResult& PotionData)
{
    // Return appropriate equipment definition based on potion properties
    // This could be a lookup table or logic based on essence/liquid type
    return BasePotionEquipmentDefinition; // Set this in the component
}
```

### Step 10: Add Equipment Manager to Character

#### Update Character Blueprint or C++

Add `UWitchPTEquipmentManagerComponent` to your character:

```cpp
// In character constructor or Blueprint
EquipmentManagerComponent = CreateDefaultSubobject<UWitchPTEquipmentManagerComponent>(TEXT("EquipmentManager"));
```

### Step 11: Create UI Integration Points

#### Widget Controller Functions

```cpp
// Add to inventory widget controller
UFUNCTION(BlueprintCallable, Category="Equipment")
void EquipInventoryItem(UWitchPTInventoryItemInstance* Item)
{
    if (UWitchPTEquipmentManagerComponent* EquipmentManager = GetEquipmentManager())
    {
        EquipmentManager->EquipInventoryItem(Item);
    }
}

UFUNCTION(BlueprintCallable, Category="Equipment")
void UnequipInventoryItem(UWitchPTInventoryItemInstance* Item)
{
    if (UWitchPTEquipmentManagerComponent* EquipmentManager = GetEquipmentManager())
    {
        EquipmentManager->UnequipInventoryItem(Item);
    }
}

UFUNCTION(BlueprintPure, Category="Equipment")
bool IsItemEquipped(UWitchPTInventoryItemInstance* Item) const
{
    if (UWitchPTEquipmentManagerComponent* EquipmentManager = GetEquipmentManager())
    {
        return EquipmentManager->IsInventoryItemEquipped(Item);
    }
    return false;
}

UWitchPTEquipmentManagerComponent* GetEquipmentManager() const
{
    // Get from character or wherever it's located
    if (APawn* PlayerPawn = GetPlayerPawn())
    {
        return PlayerPawn->FindComponentByClass<UWitchPTEquipmentManagerComponent>();
    }
    return nullptr;
}
```

---

## Phase 5: Testing and Validation

### Step 12: Create Test Assets

#### Test Equipment Definitions

1. **Create Base Potion Equipment Definition**
   - Blueprint based on `UWitchPTEquipmentDefinition`
   - Add sample abilities for testing
   - Configure any visual actors if needed

2. **Create Test Item with Equipment Fragment**
   - Create test inventory item definition
   - Add `UWitchPTInventoryItemFragment_EquippableItem` fragment
   - Set equipment definition

#### Test Scenarios

1. **Basic Equipment Flow**
   - Equip/unequip items through equipment manager
   - Verify equipment instances are created/destroyed
   - Test ability granting/removal

2. **Inventory Integration**
   - Test `EquipInventoryItem()` function
   - Verify inventory items can be equipped
   - Test equipment status queries

3. **Network Replication**
   - Test equipment on dedicated server
   - Verify clients see equipment changes
   - Test equipment instance replication

4. **Potion Integration**
   - Craft potions and verify they're equippable
   - Test potion abilities when equipped
   - Verify equipment cleanup on unequip

---

## Implementation Priority

### High Priority (Core Functionality)
1. Steps 1-3: Equipment infrastructure (definitions, instances, fragments)
2. Steps 4-6: Equipment manager implementation
3. Steps 7-8: Equipment list and ability handling

### Medium Priority (Integration)
4. Steps 9-11: Potion system integration and UI hooks

### Low Priority (Testing & Validation)
5. Step 12: Testing and validation

---

## Key Design Decisions

1. **Standalone Component**: Equipment managed by dedicated `UWitchPTEquipmentManagerComponent`
2. **Simple Ability Arrays**: Use `TArray<TSubclassOf<UGameplayAbility>>` instead of ability sets
3. **Inventory Integration**: Equipment instances reference source inventory items
4. **Authority-Only Logic**: All equipment operations require server authority
5. **Fast Array Replication**: Efficient networking through fast array serializers
6. **Flexible Equipment Types**: Support for any equipment type through definition classes

This simplified design focuses on core equipment functionality while maintaining integration points with the existing inventory and potion systems. 