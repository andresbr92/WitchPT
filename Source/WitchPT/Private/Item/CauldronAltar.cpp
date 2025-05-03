#include "Item/CauldronAltar.h"
#include "Item/CauldronPosition.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

// Sets default values
ACauldronAltar::ACauldronAltar()
{
    // Set this actor to call Tick() every frame
    PrimaryActorTick.bCanEverTick = true;

    // Make sure it replicates
    bReplicates = true;
    bAlwaysRelevant = true;
    CauldronTimer = 0.0f;
}

void ACauldronAltar::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    //
    DOREPLIFETIME(ACauldronAltar, PlayerInputSequences);
    DOREPLIFETIME(ACauldronAltar, PlayerSequenceIndices);
    DOREPLIFETIME(ACauldronAltar, IngredientTimeWindow);
    DOREPLIFETIME(ACauldronAltar, CauldronTimer);
}

// Called when the game starts or when spawned
void ACauldronAltar::BeginPlay()
{
    // Super::BeginPlay();
    //
    // // Initialize cauldron positions
    // for (auto Position : CauldronPositions)
    // {
    //     if (Position)
    //     {
    //         // Setup the position
    //         Position->SetCauldronAltar(this);
    //     }
    // }
}

// Called every frame
void ACauldronAltar::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // // Update cauldron timer if active
    // if (CurrentState == EInteractionState::Active && GetLocalRole() == ROLE_Authority)
    // {
    //     CauldronTimer -= DeltaTime;
    //     if (CauldronTimer <= 0.0f)
    //     {
    //         // Time's up!
    //         OnCauldronTimerExpired();
    //     }
    // }
}

void ACauldronAltar::StartBrewing(ACharacter* InitiatingPlayer)
{
    // if (GetLocalRole() != ROLE_Authority || CurrentState != EInteractionState::Inactive)
    // {
    //     return;
    // }
    //
    // // Make sure we have at least one player
    // if (ParticipatingPlayers.Num() == 0)
    // {
    //     // Can't start with no players
    //     UE_LOG(LogTemp, Warning, TEXT("[CauldronAltar] Cannot start brewing with no participating players"));
    //     return;
    // }
    //
    // // Change state to preparing
    // CurrentState = EInteractionState::Preparing;
    // Multicast_OnStateChanged(CurrentState);
    //
    // // Generate sequences for each player
    // GeneratePlayerSequences();
    //
    // // Transition to active state
    // CurrentState = EInteractionState::Active;
    // Multicast_OnStateChanged(CurrentState);
    //
    // // Start the brewing timer
    // StartCauldronTimer();
    //
    // UE_LOG(LogTemp, Log, TEXT("[CauldronAltar] Brewing started with %d players"), ParticipatingPlayers.Num());
}

bool ACauldronAltar::HandlePlayerInput(ACharacter* Character, const FGameplayTag& InputTag)
{
    // if (GetLocalRole() != ROLE_Authority || CurrentState != EInteractionState::Active || !Character)
    // {
    //     return false;
    // }
    //
    // // Check if this player is participating
    // if (!IsPlayerEligibleForInteraction(Character))
    // {
    //     UE_LOG(LogTemp, Warning, TEXT("[CauldronAltar] Player %s tried to input but is not eligible"), 
    //         *Character->GetName());
    //     return false;
    // }
    //
    // // Get the player's expected input
    // FGameplayTag ExpectedInput = GetCurrentExpectedInputForPlayer(Character);
    //
    // // Check if the input matches what's expected
    // bool bInputCorrect = (InputTag == ExpectedInput);
    //
    // if (bInputCorrect)
    // {
    //     // Handle successful input
    //     HandleInputSuccess(Character);
    //     
    //     // Update the player's sequence index
    //     int32& CurrentIndex = PlayerSequenceIndices.FindOrAdd(Character);
    //     CurrentIndex++;
    //     
    //     // If this player has completed their sequence, check if all players are done
    //     if (CurrentIndex >= PlayerInputSequences[Character].Ingredients.Num())
    //     {
    //         if (AreAllPlayersDone())
    //         {
    //             // All players have completed their sequences!
    //             CurrentState = EInteractionState::Succeeded;
    //             Multicast_OnStateChanged(CurrentState);
    //             
    //             // Spawn reward
    //             SpawnReward();
    //             
    //             // Clean up
    //             CleanupInteraction();
    //         }
    //     }
    //     
    //     UE_LOG(LogTemp, Log, TEXT("[CauldronAltar] Player %s input correct. Index now %d of %d"), 
    //         *Character->GetName(), CurrentIndex, PlayerInputSequences[Character].Ingredients.Num());
    //     
    //     // Broadcast the delegate
    //     OnInputReceived.Broadcast(Character, true);
    //     
    //     return true;
    // }
    // else
    // {
    //     // Handle failed input
    //     HandleInputFailure(Character);
    //     
    //     // Broadcast the delegate
    //     OnInputReceived.Broadcast(Character, false);
    //     
    //     return false;
    // }
    return false;
}

// void ACauldronAltar::OccupyPosition(ACharacter* Player, ACauldronPosition* Position)
// {
//     if (!Player || !Position || GetLocalRole() != ROLE_Authority)
//     {
//         return;
//     }
//
//     // Call the base class implementation first
//     Super::OccupyPosition(Player, Position);
//     
//     // Initialize player sequence data if not already present
//     if (!PlayerInputSequences.Contains(Player))
//     {
//         FIngredientSequence NewSequence;
//         PlayerInputSequences.Add(Player, NewSequence);
//         PlayerSequenceIndices.Add(Player, 0);
//     }
//     
//     UE_LOG(LogTemp, Log, TEXT("[CauldronAltar] Player %s occupied position %s"),
//         *Player->GetName(), *Position->GetName());
// }

int32 ACauldronAltar::GetPlayerSequenceProgress(ACharacter* Player) const
{
    // if (!Player || !PlayerInputSequences.Contains(Player) || PlayerInputSequences[Player].Ingredients.Num() == 0)
    // {
    //     return 0;
    // }
    //
    // int32 CurrentIndex = PlayerSequenceIndices.Contains(Player) ? PlayerSequenceIndices[Player] : 0;
    // return (CurrentIndex * 100) / PlayerInputSequences[Player].Ingredients.Num();
    return 1;
}

TArray<FGameplayTag> ACauldronAltar::GetPlayerSequence(ACharacter* Player) const
{
    // if (!Player || !PlayerInputSequences.Contains(Player))
    // {
    //     return TArray<FGameplayTag>();
    // }
    //
    // return PlayerInputSequences[Player].Ingredients;
    FGameplayTag Tag = FGameplayTag();
    return TArray<FGameplayTag>({Tag});
}

FGameplayTag ACauldronAltar::GetCurrentExpectedInputForPlayer(ACharacter* Player) const
{
    // if (!Player || !PlayerInputSequences.Contains(Player))
    // {
    //     return FGameplayTag();
    // }
    //
    // const TArray<FGameplayTag>& Sequence = PlayerInputSequences[Player].Ingredients;
    // int32 CurrentIndex = PlayerSequenceIndices.Contains(Player) ? PlayerSequenceIndices[Player] : 0;
    //
    // // Check if the player has completed their sequence
    // if (CurrentIndex >= Sequence.Num())
    // {
    //     return FGameplayTag();
    // }
    //
    // return Sequence[CurrentIndex];
    FGameplayTag Tag = FGameplayTag();
    return Tag;
}

void ACauldronAltar::OnRep_CauldronTimer()
{
    // Update UI or visuals based on timer
}

void ACauldronAltar::GeneratePlayerSequences()
{
    // PlayerInputSequences.Empty();
    // PlayerSequenceIndices.Empty();
    //
    // // Generate a random sequence for each player
    // for (ACharacter* Player : ParticipatingPlayers)
    // {
    //     if (!Player)
    //     {
    //         continue;
    //     }
    //     
    //     FIngredientSequence NewSequence;
    //     
    //     // Generate 3-5 random ingredients for each player
    //     int32 NumIngredients = FMath::RandRange(3, 5);
    //     for (int32 i = 0; i < NumIngredients; ++i)
    //     {
    //         ECauldronInput RandomInput = static_cast<ECauldronInput>(FMath::RandRange(0, static_cast<int32>(ECauldronInput::Ingredient5)));
    //         FGameplayTag InputTag = ConvertECauldronInputToTag(RandomInput);
    //         NewSequence.Ingredients.Add(InputTag);
    //     }
    //     
    //     // Store the sequence for this player
    //     PlayerInputSequences.Add(Player, NewSequence);
    //     PlayerSequenceIndices.Add(Player, 0);
    //     
    //     UE_LOG(LogTemp, Log, TEXT("[CauldronAltar] Generated sequence for player %s with %d ingredients"), 
    //         *Player->GetName(), NewSequence.Ingredients.Num());
    // }
}

void ACauldronAltar::HandleInputSuccess(ACharacter* Player)
{
    // Super::HandleInputSuccess(Player);
    //
    // // Play success effects
    // Multicast_OnInputSuccess(Player);
    //
    // UE_LOG(LogTemp, Log, TEXT("[CauldronAltar] Player %s successfully added an ingredient"), *Player->GetName());
}

void ACauldronAltar::HandleInputFailure(ACharacter* Player)
{
    // Super::HandleInputFailure(Player);
    //
    // // Play failure effects
    // Multicast_OnInputFailed(Player);
    //
    // UE_LOG(LogTemp, Log, TEXT("[CauldronAltar] Player %s failed to add the correct ingredient"), *Player->GetName());
}

void ACauldronAltar::StartCauldronTimer()
{
    // // Set the timer duration
    // CauldronTimer = IngredientTimeWindow;
    //
    // // Start the timer
    // GetWorldTimerManager().SetTimer(
    //     CauldronTimerHandle,
    //     this,
    //     &ACauldronAltar::OnCauldronTimerExpired,
    //     CauldronTimer,
    //     false
    // );
    //
    // UE_LOG(LogTemp, Log, TEXT("[CauldronAltar] Started brewing timer: %.1f seconds"), CauldronTimer);
}

void ACauldronAltar::OnCauldronTimerExpired()
{
    // if (GetLocalRole() != ROLE_Authority || CurrentState != EInteractionState::Active)
    // {
    //     return;
    // }
    //
    // // Check if all players have completed their sequences
    // if (AreAllPlayersDone())
    // {
    //     // Success!
    //     CurrentState = EInteractionState::Succeeded;
    //     Multicast_OnStateChanged(CurrentState);
    //     
    //     // Spawn reward
    //     SpawnReward();
    //     
    //     // Clean up
    //     CleanupInteraction();
    //     
    //     UE_LOG(LogTemp, Log, TEXT("[CauldronAltar] Brewing succeeded - all players completed in time"));
    // }
    // else
    // {
    //     // Failure - time ran out
    //     CurrentState = EInteractionState::Failed;
    //     Multicast_OnStateChanged(CurrentState);
    //     
    //     // Apply aging penalties to all players who didn't complete
    //     for (ACharacter* Player : ParticipatingPlayers)
    //     {
    //         if (Player && PlayerSequenceIndices.Contains(Player))
    //         {
    //             int32 CurrentIndex = PlayerSequenceIndices[Player];
    //             TArray<FGameplayTag> Sequence = PlayerInputSequences[Player].Ingredients;
    //             
    //             // Check if this player didn't complete their sequence
    //             if (CurrentIndex < Sequence.Num())
    //             {
    //                 ApplyAgePenalty(Player, false);
    //             }
    //         }
    //     }
    //     
    //     // Clean up
    //     CleanupInteraction();
    //     
    //     UE_LOG(LogTemp, Log, TEXT("[CauldronAltar] Brewing failed - time ran out"));
    // }
}

FGameplayTag ACauldronAltar::ConvertECauldronInputToTag(ECauldronInput Input)
{
    // FString TagString;
    //
    // switch (Input)
    // {
    // case ECauldronInput::Ingredient1:
    //     TagString = "Input.Cauldron.Ingredient1";
    //     break;
    // case ECauldronInput::Ingredient2:
    //     TagString = "Input.Cauldron.Ingredient2";
    //     break;
    // case ECauldronInput::Ingredient3:
    //     TagString = "Input.Cauldron.Ingredient3";
    //     break;
    // case ECauldronInput::Ingredient4:
    //     TagString = "Input.Cauldron.Ingredient4";
    //     break;
    // case ECauldronInput::Ingredient5:
    //     TagString = "Input.Cauldron.Ingredient5";
    //     break;
    // default:
    //     TagString = "Input.Cauldron.None";
    //     break;
    // }
    //
    FGameplayTag Tag = FGameplayTag();
    return Tag;
}

ECauldronInput ACauldronAltar::ConvertTagToECauldronInput(const FGameplayTag& Tag)
{
    // FString TagString = Tag.ToString();
    //
    // if (TagString.Contains("Ingredient1"))
    // {
    //     return ECauldronInput::Ingredient1;
    // }
    // else if (TagString.Contains("Ingredient2"))
    // {
    //     return ECauldronInput::Ingredient2;
    // }
    // else if (TagString.Contains("Ingredient3"))
    // {
    //     return ECauldronInput::Ingredient3;
    // }
    // else if (TagString.Contains("Ingredient4"))
    // {
    //     return ECauldronInput::Ingredient4;
    // }
    // else if (TagString.Contains("Ingredient5"))
    // {
    //     return ECauldronInput::Ingredient5;
    // }
    //
    return ECauldronInput::None;
}

bool ACauldronAltar::IsPlayerEligibleForInteraction(ACharacter* Player) const
{
    return Super::IsPlayerEligibleForInteraction(Player) && PlayerInputSequences.Contains(Player);
}

bool ACauldronAltar::AreAllPlayersDone() const
{
    // for (ACharacter* Player : ParticipatingPlayers)
    // {
    //     if (!Player || !PlayerInputSequences.Contains(Player) || !PlayerSequenceIndices.Contains(Player))
    //     {
    //         continue;
    //     }
    //     
    //     int32 CurrentIndex = PlayerSequenceIndices[Player];
    //     int32 SequenceLength = PlayerInputSequences[Player].Ingredients.Num();
    //     
    //     // If any player hasn't completed their sequence, return false
    //     if (CurrentIndex < SequenceLength)
    //     {
    //         return false;
    //     }
    // }
    //
    // // All players have completed their sequences
    // return true;
    return true;
} 