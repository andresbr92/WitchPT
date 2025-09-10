// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/Components/CraftComponent.h"

#include "AsyncGameplayMessageSystem.h"
#include "AsyncMessageWorldSubsystem.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "FWitchPTGameplayTags.h"
#include "Inventory/WitchPTInventoryItemDefinition.h"
#include "Inventory/WitchPTInventoryItemInstance.h"
#include "Inventory/WitchPTInventoryManagerComponent.h"
#include "Inventory/Fragments/InventoryFragment_UIDetails.h"
#include "Inventory/Fragments/InventoryFragment_IngredientCraftingProperties.h"
#include "Inventory/Fragments/InventoryFragment_PotionProperties.h"
#include "Inventory/Fragments/InventoryFragment_WorldDetails.h"
#include "Player/WitchPTPlayerController.h"
#include "Item/CauldronAltar.h"
#include "Item/Potion/PotionBase.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/ActorChannel.h"
#include "Engine/World.h"
#include "Inventory/Fragments/InventoryFragment_EquippableItem.h"

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
    if (UCraftComponent* CraftComponent = Cast<UCraftComponent>(OwnerComponent))
    {
        
        switch (SlotIndex)
        {
        case 0: // Base Ingredient
            
            break;
        case 1: // Principal Ingredient
            
            break;
        case 2: // Modifier Ingredient
           
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
    for (const FCauldronIngredientEntry& Entry : Entries)
    {
        if (Entry.SlotIndex == SlotIndex)
        {
            return Entry.IngredientInstance;
        }
    }
    return nullptr;
}


UCraftComponent::UCraftComponent()
	: IngredientList(this)
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
    bReplicateUsingRegisteredSubObjectList = true;
}

// Called when the game starts
void UCraftComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UCraftComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCraftComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ThisClass, IngredientList);
}

bool UCraftComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch,
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

void UCraftComponent::ReadyForReplication()
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

void UCraftComponent::TryAddIngredient(const ACharacter* RequestingCharacter, UWitchPTInventoryItemInstance* IngredientInstance)
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
    
    

   
    
    const UInventoryFragment_IngredientCraftingProperties* IngredientCraftingDetails = Cast<UInventoryFragment_IngredientCraftingProperties>(IngredientInstance->FindFragmentByClass(UInventoryFragment_IngredientCraftingProperties::StaticClass()));
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

    
    if (!bIsBaseIngredient && !bIsPrincipalIngredient && !bIsModifierIngredient)
    {
        UE_LOG(LogTemp, Warning, TEXT("UCauldronCraftComponent::TrySetIngredientInSlot: Ingredient %s doesn't have a valid slot usage tag"), *IngredientInstance->GetName());
        return;
    }
    
    bool bConsumedSuccessfully = false;
    int32 CurrentStackCount = 3;
    
    if (CurrentStackCount > 0)
    {
        
        
        if (CurrentStackCount == 1)
        {
            
            // InventoryManager->Server_RemoveItemInstance(IngredientInstance);
            bConsumedSuccessfully = true;
        }
        else
        {
            // InventoryManager->Server_UpdateItemStackCount(IngredientInstance, CurrentStackCount - 1);
            
            bConsumedSuccessfully = true;
        }
        
        // Get UI fragment for icon
        const UInventoryFragment_UIDetails* UIFragment = Cast<UInventoryFragment_UIDetails>(IngredientInstance->FindFragmentByClass(UInventoryFragment_UIDetails::StaticClass()));
        TSubclassOf<UUserWidget> IngredientIcon = nullptr;
        if (IsValid(UIFragment))
        {
            IngredientIcon = UIFragment->IconWidget;
        }
        
        // Set the ingredient in the appropriate slot
        if (bIsBaseIngredient)
        {
            IngredientList.SetIngredientAtSlot(0, IngredientInstance);
            
            UE_LOG(LogTemp, Log, TEXT("UCauldronCraftComponent::TrySetIngredientInSlot: Base ingredient set successfully"));
            
            if (GetOwner()->HasAuthority())
            {
               
            }
        }
        else if (bIsPrincipalIngredient)
        {
            IngredientList.SetIngredientAtSlot(1, IngredientInstance);
           
            UE_LOG(LogTemp, Log, TEXT("UCauldronCraftComponent::TrySetIngredientInSlot: Principal ingredient set successfully"));
            
            if (GetOwner()->HasAuthority())
            {
            }
        }
        else if (bIsModifierIngredient)
        {
            IngredientList.SetIngredientAtSlot(2, IngredientInstance);
         
            UE_LOG(LogTemp, Log, TEXT("UCauldronCraftComponent::TrySetIngredientInSlot: Modifier ingredient set successfully"));
            
            if (GetOwner()->HasAuthority())
            {
            }
        }
    }
    TSharedPtr<FAsyncMessageSystemBase> MessageSystem = UAsyncMessageWorldSubsystem::GetSharedMessageSystem(GetWorld());
    FAsyncMessageId MessageId = FAsyncMessageId(FWitchPTGameplayTags::Get().Event_Cauldron_Ingredient_Added);
    FIngredientSetPayload Payload;
    Payload.ItemInstance = IngredientInstance;
    FInstancedStruct PayloadStruct = FInstancedStruct::Make(Payload);
    MessageSystem->QueueMessageForBroadcast(MessageId, PayloadStruct);
}

void UCraftComponent::CraftPotion(const ACharacter* RequestingCharacter) const
{
    // validate if at least base and principal ingredients are set
    UWitchPTInventoryItemInstance* BaseIngredient = IngredientList.GetIngredientAtSlot(0);
    UWitchPTInventoryItemInstance* PrincipalIngredient = IngredientList.GetIngredientAtSlot(1);
    UWitchPTInventoryItemInstance* ModifierIngredient = IngredientList.GetIngredientAtSlot(2);
    if (!BaseIngredient || !PrincipalIngredient)
    {
        UE_LOG(LogTemp, Warning, TEXT("UCauldronCraftComponent::CraftPotion: Base or Principal ingredient is missing."));
        return;
    }
    // Call crafting manager to craft the potion
    if (UCraftingManagerSubsystem* CMSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UCraftingManagerSubsystem>())
    {
        FCalculatedPotionStats PotionStats = CMSubsystem->CraftPotion(BaseIngredient, PrincipalIngredient, ModifierIngredient);
        // log the result
        if (!PotionStats.bIsValidPotion)
        {
            UE_LOG(LogTemp, Warning, TEXT("UCauldronCraftComponent::CraftPotion: The combination of ingredients did not produce a valid potion."));
            return;
        } else
        {
            UE_LOG(LogTemp, Log, TEXT("UCauldronCraftComponent::CraftPotion: Potion crafted successfully with Name: %s, Description: %s, Potency: %f, Duration: %f, Charges: %d"),
                   *PotionStats.Name.ToString(), *PotionStats.Description.ToString(), PotionStats.FinalPotency, PotionStats.FinalDuration, PotionStats.FinalCharges);
            APotionBase* NewPotion = NewObject<APotionBase>(PotionActorClass);
            
            
        }
    }
}

void UCraftComponent::GetIngredientAtSlot(int32 SlotIndex,
                                          UWitchPTInventoryItemInstance*& OutIngredientInstance) const
{
    OutIngredientInstance = IngredientList.GetIngredientAtSlot(SlotIndex);
}




