// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/Components/CauldronCraftComponent.h"

#include "AsyncGameplayMessageSystem.h"
#include "AsyncMessageWorldSubsystem.h"
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
#include "Engine/ActorChannel.h"
#include "Engine/World.h"

void FCauldronIngredientList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
    for (int32 Index : RemovedIndices)
    {
        if (Entries.IsValidIndex(Index))
        {
            const FCauldronIngredientEntry& Entry = Entries[Index];
            NotifyIngredientChanged(Entry.SlotIndex, nullptr); // nullptr indica que se removió
        }
    }
}

void FCauldronIngredientList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
    for (int32 Index : AddedIndices)
    {
        if (Entries.IsValidIndex(Index))
        {
            const FCauldronIngredientEntry& Entry = Entries[Index];
            NotifyIngredientChanged(Entry.SlotIndex, Entry.IngredientInstance);
        }
    }
}

void FCauldronIngredientList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
    for (int32 Index : ChangedIndices)
    {
        if (Entries.IsValidIndex(Index))
        {
            const FCauldronIngredientEntry& Entry = Entries[Index];
            NotifyIngredientChanged(Entry.SlotIndex, Entry.IngredientInstance);
        }
    }
}

void FCauldronIngredientList::NotifyIngredientChanged(int32 SlotIndex, UWitchPTInventoryItemInstance* Instance)
{
    if (UCauldronCraftComponent* CraftComponent = Cast<UCauldronCraftComponent>(OwnerComponent))
    {
        // Disparar el delegate apropiado según el slot
        switch (SlotIndex)
        {
        case 0: // Base Ingredient
            CraftComponent->OnBaseIngredientSetDelegate.Broadcast(Instance);
            break;
        case 1: // Principal Ingredient
            CraftComponent->OnPrincipalIngredientSetDelegate.Broadcast(Instance);
            break;
        case 2: // Modifier Ingredient
            CraftComponent->OnModifierIngredientSetDelegate.Broadcast(Instance);
            break;
        default: ;
        }
    }
}

void FCauldronIngredientList::SetIngredientAtSlot(int32 SlotIndex, UWitchPTInventoryItemInstance* Instance)
{
    for (FCauldronIngredientEntry& Entry : Entries)
    {
        if (Entry.SlotIndex == SlotIndex)
        {
            
            Entry.IngredientInstance = Instance;
            MarkItemDirty(Entry); 
            return;
        }
    }
    
    // Si no existe el slot, crear nueva entrada
    FCauldronIngredientEntry& NewEntry = Entries.AddDefaulted_GetRef();
    NewEntry.SlotIndex = SlotIndex;
    NewEntry.IngredientInstance = Instance;
    MarkArrayDirty();
}

UWitchPTInventoryItemInstance* FCauldronIngredientList::GetIngredientAtSlot(int32 SlotIndex) const
{
    return nullptr;
}


UCauldronCraftComponent::UCauldronCraftComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
    bReplicateUsingRegisteredSubObjectList = true;
    IngredientList.OwnerComponent = this;

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
    DOREPLIFETIME(UCauldronCraftComponent, IngredientList);

}

bool UCauldronCraftComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch,
    FReplicationFlags* RepFlags)
{
    bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

    for (FCauldronIngredientEntry& Entry : IngredientList.Entries)
    {
        UWitchPTInventoryItemInstance* Instance = Entry.IngredientInstance;

        if (Instance && IsValid(Instance))
        {
            WroteSomething |= Channel->ReplicateSubobject(Instance, *Bunch, *RepFlags);
        }
    }

    return WroteSomething;
}

void UCauldronCraftComponent::ReadyForReplication()
{
    Super::ReadyForReplication();
    if (IsUsingRegisteredSubObjectList())
    {
        for (const FCauldronIngredientEntry& Entry : IngredientList.Entries)
        {
            UWitchPTInventoryItemInstance* Instance = Entry.IngredientInstance;

            if (IsValid(Instance))
            {
                AddReplicatedSubObject(Instance);
            }
        }
    }
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
    bool bIsBaseIngredient = IngredientCraftingDetails->SlotUsageTag.MatchesTag(GameplayTags.Ingredient_Base);
    bool bIsPrincipalIngredient = IngredientCraftingDetails->SlotUsageTag.MatchesTag(GameplayTags.Ingredient_Principal);
    bool bIsModifierIngredient = IngredientCraftingDetails->SlotUsageTag.MatchesTag(GameplayTags.Ingredient_Modifier);

    // // Check if the appropriate slot is already occupied
    // if (bIsBaseIngredient && BaseIngredient != nullptr)
    // {
    //     UE_LOG(LogTemp, Warning, TEXT("UCauldronCraftComponent::TrySetIngredientInSlot: Base ingredient slot is already occupied"));
    //     return;
    // }
    //
    // if (bIsPrincipalIngredient && PrincipalIngredient != nullptr)
    // {
    //     UE_LOG(LogTemp, Warning, TEXT("UCauldronCraftComponent::TrySetIngredientInSlot: Principal ingredient slot is already occupied"));
    //     return;
    // }
    //
    // if (bIsModifierIngredient && ModifierIngredient != nullptr)
    // {
    //     UE_LOG(LogTemp, Warning, TEXT("UCauldronCraftComponent::TrySetIngredientInSlot: Modifier ingredient slot is already occupied"));
    //     return;
    // }

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
            IngredientList.SetIngredientAtSlot(0, IngredientToSet);
            
            UE_LOG(LogTemp, Log, TEXT("UCauldronCraftComponent::TrySetIngredientInSlot: Base ingredient set successfully"));
            
            if (GetOwner()->HasAuthority())
            {
                BroadcastBaseIngredientIconSet();
            }
        }
        else if (bIsPrincipalIngredient)
        {
            IngredientList.SetIngredientAtSlot(1, IngredientToSet);
           
            UE_LOG(LogTemp, Log, TEXT("UCauldronCraftComponent::TrySetIngredientInSlot: Principal ingredient set successfully"));
            
            if (GetOwner()->HasAuthority())
            {
                BroadcastPrincipalIngredientIconSet();
            }
        }
        else if (bIsModifierIngredient)
        {
            IngredientList.SetIngredientAtSlot(2, IngredientToSet);
         
            UE_LOG(LogTemp, Log, TEXT("UCauldronCraftComponent::TrySetIngredientInSlot: Modifier ingredient set successfully"));
            
            if (GetOwner()->HasAuthority())
            {
                BroadcastModifierIngredientIconSet();
            }
        }
    }
    TSharedPtr<FAsyncMessageSystemBase> MessageSystem = UAsyncMessageWorldSubsystem::GetSharedMessageSystem(GetWorld());
    FAsyncMessageId MessageId = FAsyncMessageId(FWitchPTGameplayTags::Get().Event_Cauldron_Ingredient_Added);
    FIngredientSetPayload Payload;
    Payload.ItemInstance = IngredientItemDef;
    FInstancedStruct PayloadStruct = FInstancedStruct::Make(Payload);
    MessageSystem->QueueMessageForBroadcast(MessageId, PayloadStruct);
}

void UCauldronCraftComponent::GetIngredientAtSlot(int32 SlotIndex,
    UWitchPTInventoryItemInstance*& OutIngredientInstance) const
{
    OutIngredientInstance = IngredientList.GetIngredientAtSlot(0);
}

// ----------------------------------- BROADCAST HELPER FUNCTIONS ---------------------------------------------- //
void UCauldronCraftComponent::BroadcastBaseIngredientDropped() const
{
    // OnBaseIngredientSetDelegate.Broadcast(BaseIngredient);
}

void UCauldronCraftComponent::BroadcastBaseIngredientIconSet() const
{
    
}

void UCauldronCraftComponent::BroadcastPrincipalIngredientDropped() const
{
    // OnPrincipalIngredientSetDelegate.Broadcast(PrincipalIngredient);
}

void UCauldronCraftComponent::BroadcastPrincipalIngredientIconSet() const
{
    
}

void UCauldronCraftComponent::BroadcastModifierIngredientDropped() const
{
    // OnModifierIngredientSetDelegate.Broadcast(ModifierIngredient);
}

void UCauldronCraftComponent::BroadcastModifierIngredientIconSet() const
{
    
    
}



