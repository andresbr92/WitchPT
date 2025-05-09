#include "Item/BaseInteractionPosition.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"
#include "AbilitySystemComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"

ABaseInteractionPosition::ABaseInteractionPosition()
{
    // Set this actor to call Tick() every frame
    PrimaryActorTick.bCanEverTick = true;
    
    // Make sure this actor replicates
    bReplicates = true;
    bAlwaysRelevant = true;
}

void ABaseInteractionPosition::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    // Replicate the occupied state and occupying character
    DOREPLIFETIME(ABaseInteractionPosition, bIsOccupied);
    DOREPLIFETIME(ABaseInteractionPosition, OccupyingCharacter);
    DOREPLIFETIME(ABaseInteractionPosition, PositionTag);
    
    // PositionTag and Altar are usually set once and don't need replication unless they can change dynamically
}

void ABaseInteractionPosition::BeginPlay()
{
    Super::BeginPlay();

    // Additional initialization could go here if needed
}

void ABaseInteractionPosition::GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& OptionBuilder)
{
    // This should be overridden by derived classes
    Super::GatherInteractionOptions(InteractQuery, OptionBuilder);
}

bool ABaseInteractionPosition::IsOccupied() const
{
    return bIsOccupied;
}

void ABaseInteractionPosition::SetOccupied(ACharacter* Character)
{
    if (GetLocalRole() != ROLE_Authority)
    {
        return;
    }

    OccupyingCharacter = Character;
    bIsOccupied = (Character != nullptr);
}

void ABaseInteractionPosition::SetUnoccupied()
{
    if (GetLocalRole() != ROLE_Authority)
    {
        return;
    }

    RemoveCharacterFromPosition();
}

void ABaseInteractionPosition::SetPositionTag(const FGameplayTag& NewTag)
{
    PositionTag = NewTag;
}

void ABaseInteractionPosition::OnRep_IsOccupied()
{
    // Handle any visuals or sounds when occupation state changes
}

void ABaseInteractionPosition::OnRep_OccupyingCharacter()
{
    // Handle any visuals or sounds when the occupying character changes
}

void ABaseInteractionPosition::HandleInteraction(ACharacter* InteractingCharacter)
{
    // Base implementation - should be overridden by child classes
    UE_LOG(LogTemp, Log, TEXT("[BaseInteractionPosition] %s handling interaction with %s"),
        *GetName(), *InteractingCharacter->GetName());
}

void ABaseInteractionPosition::RemoveCharacterFromPosition()
{
    OccupyingCharacter = nullptr;
    bIsOccupied = false;
} 