#include "Item/BaseInteractableAltar.h"

#include "FWitchPTGameplayTags.h"
#include "Item/BaseInteractionPosition.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"

// Sets default values
ABaseInteractableAltar::ABaseInteractableAltar()
{
    // Set this actor to call Tick() every frame
    PrimaryActorTick.bCanEverTick = true;

    // Make sure it replicates
    bReplicates = true;
    bAlwaysRelevant = true;
    
    TagsPositions.Add(FWitchPTGameplayTags::Get().Ritual_Position_1);
    TagsPositions.Add(FWitchPTGameplayTags::Get().Ritual_Position_2);
    TagsPositions.Add(FWitchPTGameplayTags::Get().Ritual_Position_3);
    TagsPositions.Add(FWitchPTGameplayTags::Get().Ritual_Position_4);

}

void ABaseInteractableAltar::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ABaseInteractableAltar, CurrentState);
    DOREPLIFETIME(ABaseInteractableAltar, ParticipatingPlayers);
  
}

// Called when the game starts or when spawned
void ABaseInteractableAltar::BeginPlay()
{
    Super::BeginPlay();
    
    // Initialize interaction positions
    CreateAltarPositions();
}

// Called every frame
void ABaseInteractableAltar::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Child classes will implement specific tick behavior
}

void ABaseInteractableAltar::OccupyPosition(ACharacter* Player, ABaseInteractionPosition* Position)
{
    if (!Player || !Position || GetLocalRole() != ROLE_Authority)
    {
        return;
    }

    // Set the position as occupied
    Position->Server_SetOccupied(Player);
    
    // Track which position this player is at
    PlayerPositionTags.Add(Player, Position->GetPositionTag());
    
    // Add to participating players if not already there
    if (!ParticipatingPlayers.Contains(Player))
    {
        ParticipatingPlayers.Add(Player);
    }
    
    UE_LOG(LogTemp, Log, TEXT("[BaseInteractableAltar] Player %s occupied position %s"),
        *Player->GetName(), *Position->GetName());
        
    // Child classes can override to add additional logic
}

void ABaseInteractableAltar::CreateAltarPositions()
{
    if (!HasAuthority() || !PositionClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("ACauldronAltar::CreateBrewingPositions: Missing authority or position class"));
        return;
    }
    
    // Make sure we have no previous positions
    DestroyAltarPositions();
    
    UE_LOG(LogTemp, Log, TEXT("ACauldronAltar::CreateBrewingPositions: Creating %d brewing positions"), PositionTransforms.Num());
    int positionIndex = 0;
    // Spawn a CauldronPosition for each transform
    for (const FTransform& PosTransform : PositionTransforms)
    {
        // Transform relative to world space
        FTransform WorldTransform = PosTransform * GetActorTransform();
        
        // Get location and rotation from the transform
        FVector Location = WorldTransform.GetLocation();
        FRotator Rotation = WorldTransform.Rotator();
        
        // Spawn parameters
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
        
        // Spawn the brewing position
        ABaseInteractionPosition* NewPosition = GetWorld()->SpawnActor<ABaseInteractionPosition>(
            PositionClass, 
            Location, 
            Rotation, 
            SpawnParams);
        
        if (NewPosition)
        {
            // Initialize the position
            // Note: Implement this method in CauldronPosition if needed
            // NewPosition->SetCauldronAltar(this);
            NewPosition->SetPositionTag(TagsPositions[positionIndex]);
            positionIndex++;

            
            // Add to our array
            InteractionPositions.Add(NewPosition);
            
            UE_LOG(LogTemp, Log, TEXT("ACauldronAltar::CreateBrewingPositions: Created position at %s"), *Location.ToString());
        }
        
      
    }
   
}



void ABaseInteractableAltar::DestroyAltarPositions()
{
    // Only destroy positions on the server
    if (!HasAuthority())
    {
        return;
    }
    
    // Destroy all brewing positions
    for (ABaseInteractionPosition* Position : InteractionPositions)
    {
        if (Position)
        {
            Position->Destroy();
        }
    }
    
    // Clear the array
    InteractionPositions.Empty();
    
    UE_LOG(LogTemp, Log, TEXT("Altar::DestroyAltarPositions: All altar positions destroyed"));
}

void ABaseInteractableAltar::Multicast_OnStateChanged_Implementation(EInteractionState NewState)
{
    // Update visuals or play sounds based on state change
    UE_LOG(LogTemp, Log, TEXT("[BaseInteractableAltar] State changed to %d"), static_cast<int32>(NewState));
}

void ABaseInteractableAltar::Multicast_OnInputSuccess_Implementation(ACharacter* Character)
{
    // Play success feedback
    UE_LOG(LogTemp, Log, TEXT("[BaseInteractableAltar] Input success feedback for player %s"), *Character->GetName());
}

void ABaseInteractableAltar::Multicast_OnInputFailed_Implementation(ACharacter* Character)
{
    // Play failure feedback
    UE_LOG(LogTemp, Log, TEXT("[BaseInteractableAltar] Input failed feedback for player %s"), *Character->GetName());
}

void ABaseInteractableAltar::Multicast_OnInteractionSucceeded_Implementation()
{
    // Play overall success effects
    UE_LOG(LogTemp, Log, TEXT("[BaseInteractableAltar] Interaction succeeded feedback"));
}

void ABaseInteractableAltar::Multicast_OnInteractionCatastrophicFail_Implementation()
{
    // Play catastrophic fail effects
    UE_LOG(LogTemp, Log, TEXT("[BaseInteractableAltar] Interaction catastrophically failed feedback"));
}

void ABaseInteractableAltar::OnRep_CurrentState()
{
    // Update client-side visuals based on state
}

void ABaseInteractableAltar::OnRep_ParticipatingPlayers()
{
    // Update client-side UI or visuals for players
}


void ABaseInteractableAltar::HandleInputSuccess(ACharacter* Player)
{
    // Base implementation - to be overridden by child classes
    UE_LOG(LogTemp, Log, TEXT("[BaseInteractableAltar] Handling successful input from %s"), *Player->GetName());
}

void ABaseInteractableAltar::HandleInputFailure(ACharacter* Player)
{
   
}

void ABaseInteractableAltar::ApplyAgePenalty(ACharacter* Player, bool bCatastrophic)
{
   
}

void ABaseInteractableAltar::CheckState()
{
    // Base implementation - to be overridden by child classes
    // This would check if the interaction is complete or failed
}

void ABaseInteractableAltar::CleanupInteraction()
{
   
}

void ABaseInteractableAltar::SpawnReward()
{
   
}

void ABaseInteractableAltar::SpawnDemon()
{
    
}

bool ABaseInteractableAltar::IsPlayerEligibleForInteraction(ACharacter* Player) const
{
    // Base implementation - to be overridden by child classes
    // Check if the player is valid and in the participating list
    return Player && ParticipatingPlayers.Contains(Player);
} 