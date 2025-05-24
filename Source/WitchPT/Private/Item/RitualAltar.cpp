// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/RitualAltar.h"

#include "Net/UnrealNetwork.h"
#include "WitchPT/WitchPT.h"
#include "Item/RitualPosition.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffect.h"
#include "FWitchPTGameplayTags.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/WitchPTPlayerController.h"

// Sets default values
ARitualAltar::ARitualAltar()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true; 

	bReplicates = true;
	
	
	// Default values
	CurrentRitualState = EInteractionState::Inactive;
	CurrentSequenceIndex = -1;
	CurrentInputTimer = 0.0f;
	CorruptionAmount = 0.0f;
	MaxCorruption = 100.0f;
	CorruptionIncreasePerFail = 10.0f;
	BaseInputTimeWindow = 20.f;
	DifficultyScalingMultiplier = 1.0f;
	StartCountdown = 3;
}

void ARitualAltar::BeginPlay()
{
	Super::BeginPlay();
	
	// // Auto-discover RitualPositions if not set in editor
	// if (RitualPositions.Num() == 0 && HasAuthority())
	// {
	// 	TArray<AActor*> FoundActors;
	// 	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARitualPosition::StaticClass(), FoundActors);
	// 	
	// 	for (AActor* Actor : FoundActors)
	// 	{
	// 		ARitualPosition* Position = Cast<ARitualPosition>(Actor);
	// 		if (Position)
	// 		{
	// 			RitualPositions.Add(Position);
	// 			Position->SetRitualAltar(this);
	// 		}
	// 	}
	// }
}

void ARitualAltar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Update timer on server only
	if (HasAuthority() && CurrentRitualState == EInteractionState::Active)
	{
		// Timer logic is handled by UE's timer system, but we could add additional per-tick logic here if needed
	}
}

void ARitualAltar::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARitualAltar, CurrentRitualState);
	DOREPLIFETIME(ARitualAltar, InputSequence);
	DOREPLIFETIME(ARitualAltar, CurrentSequenceIndex);
	DOREPLIFETIME(ARitualAltar, ParticipatingPlayers);
	DOREPLIFETIME(ARitualAltar, CurrentActivePlayer);
	DOREPLIFETIME(ARitualAltar, CurrentInputTimer);
	DOREPLIFETIME(ARitualAltar, CorruptionAmount);
	DOREPLIFETIME(ARitualAltar, MaxCorruption);
	DOREPLIFETIME(ARitualAltar, CorruptionIncreasePerFail);
	DOREPLIFETIME(ARitualAltar, BaseInputTimeWindow);
	DOREPLIFETIME(ARitualAltar, DifficultyScalingMultiplier);
	DOREPLIFETIME(ARitualAltar, ReadyPlayers);
	DOREPLIFETIME(ARitualAltar, StartCountdown);
}

void ARitualAltar::Multicast_NumberOfPlayersReadyHasChanged_Implementation(int32 TotalPlayers, int32 PlayersReady)
{
	OnNumberOfReadyPlayersHasChangedDelegate.ExecuteIfBound(TotalPlayers, PlayersReady);
}

// void ARitualAltar::Multicast_CurrentActivePlayerChanged_Implementation(const ACharacter* Character)
// {
// 	OnCurrentActivePlayerChangedDelegate.ExecuteIfBound(Character, InputSequence[CurrentSequenceIndex]);
// 	
// }

void ARitualAltar::Multicast_CurrentSequenceIndexChanged_Implementation(int32 SequenceIndex)
{
	OnCurrentSequenceIndexChangedDelegate.ExecuteIfBound(SequenceIndex);
}

void ARitualAltar::StartRitual(ACharacter* RequestingCharacter)
{
	if (!HasAuthority() || !RequestingCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("[DEBUG-RITUAL] Cannot start ritual: invalid player or no authority"));
		return;
	}
	
	// Check if the ritual is in a valid state
	if (CurrentRitualState != EInteractionState::Inactive && 
	    CurrentRitualState != EInteractionState::WaitingForPlayers)
	{
		UE_LOG(LogTemp, Warning, TEXT("[DEBUG-RITUAL] Cannot start ritual: already in state %d"), static_cast<int32>(CurrentRitualState));
		return;
	}
	
	// Process the ready request
	ProcessRitualReadyRequest(RequestingCharacter);
}

void ARitualAltar::ProcessRitualReadyRequest(ACharacter* RequestingCharacter)
{
	if (!HasAuthority() || !RequestingCharacter)
	{
		return;
	}
	
	// Check if player is already in the ready list
	if (ReadyPlayers.Contains(RequestingCharacter))
	{
		// Player is already ready, could allow them to un-ready if desired
		ReadyPlayers.Remove(RequestingCharacter);
		Multicast_NumberOfPlayersReadyHasChanged(ParticipatingPlayers.Num(), ReadyPlayers.Num());
		UE_LOG(LogTemp, Log, TEXT("[DEBUG-RITUAL] Player %s canceled ready status"), *RequestingCharacter->GetName());
		return;
	}
	
	// Add player to ready list
	ReadyPlayers.Add(RequestingCharacter);
	Multicast_NumberOfPlayersReadyHasChanged(ParticipatingPlayers.Num(), ReadyPlayers.Num());
	UE_LOG(LogTemp, Log, TEXT("[DEBUG-RITUAL] Player %s is ready"), *RequestingCharacter->GetName());
	
	// Check if all players are ready
	if (AreAllPlayersReady())
	{
		UE_LOG(LogTemp, Log, TEXT("[DEBUG-RITUAL] All players are ready. Starting countdown!"));
		StartRitualCountdown();
	}
}

bool ARitualAltar::IsPlayerReady(ACharacter* Player) const
{
	return ReadyPlayers.Contains(Player);
}



bool ARitualAltar::AreAllPlayersReady() const
{
	// Check if all participating players are in the ready list
	if (ParticipatingPlayers.Num() == 0 || ReadyPlayers.Num() == 0)
	{
		return false;
	}
	
	for (ACharacter* Player : ParticipatingPlayers)
	{
		if (!ReadyPlayers.Contains(Player))
		{
			return false;
		}
	}
	
	return true;
}

void ARitualAltar::StartRitualCountdown()
{
	if (!HasAuthority())
	{
		return;
	}
	
	// Set the state to preparing
	CurrentRitualState = EInteractionState::Preparing;
	OnRitualStateChangedDelegate.ExecuteIfBound(CurrentRitualState);


	// Generate the ritual input sequence
	GenerateInputSequence();
	
	// Reset countdown value
	StartCountdown = 3;
	
	// Broadcast first countdown value
	Multicast_OnCountdownTick(StartCountdown);
	
	// Start countdown timer
	GetWorldTimerManager().SetTimer(
		RitualStartCountdownHandle,
		this,
		&ARitualAltar::ProcessCountdownTick,
		1.0f,  // Fire every second
		true   // Looping
	);
}

void ARitualAltar::ProcessCountdownTick()
{
	if (!HasAuthority())
	{
		return;
	}
	
	// Decrease countdown
	StartCountdown--;
	
	
	UE_LOG(LogTemp, Log, TEXT("[DEBUG-RITUAL] Countdown: %d"), StartCountdown);
	
	
	if (StartCountdown <= 0)
	{
		GetWorldTimerManager().ClearTimer(RitualStartCountdownHandle);
		ActivateRitual();
	}
}

void ARitualAltar::ActivateRitual()
{
	if (!HasAuthority())
	{
		return;
	}
	
	// Set initial active player (from ready list)
	int32 RandomStartingPlayer = FMath::RandRange(0, ReadyPlayers.Num() - 1);
	
	// Fallback
	CurrentActivePlayer = ParticipatingPlayers[RandomStartingPlayer];
	CurrentSequenceIndex = 0;

	FUIRitualData UIRitualData;
	
	UIRitualData.RitualPercentageCompleted = GetCurrentSequenceProgress();
	UIRitualData.CorruptionPercentage = GetCorruptionPercentage();

	if (CurrentActivePlayer->IsLocallyControlled() && CurrentActivePlayer->HasAuthority())
	{
		UIRitualData.bIsMyTurn = true;
		UIRitualData.ExpectedInput = InputSequence[CurrentSequenceIndex];
		OnIsMyTurnChangedDelegate.ExecuteIfBound(UIRitualData);
	} else 
	{
		UIRitualData.bIsMyTurn = false;
		UIRitualData.ExpectedInput = FGameplayTag::EmptyTag;
		OnIsMyTurnChangedDelegate.ExecuteIfBound(UIRitualData);
	}
	
	
	CurrentRitualState = EInteractionState::Active;
	OnRitualStateChangedDelegate.ExecuteIfBound(CurrentRitualState);
	

	
	
	// Start the input timer
	StartInputTimer();
	

}





void ARitualAltar::Multicast_OnCountdownTick_Implementation(int32 CountdownValue)
{
	// Fire delegate for UI updates
	OnRitualCountdownTickDelegate.ExecuteIfBound(CountdownValue);
}

void ARitualAltar::GenerateInputSequence()
{
	if (!HasAuthority())
	{
		return;
	}
	
	InputSequence.Empty();
	
	// Number of inputs scales with the number of players and difficulty
	int32 SequenceLength = FMath::Max(5, FMath::RoundToInt(5.0f * ParticipatingPlayers.Num() * DifficultyScalingMultiplier));
	
	// Get gameplay tags from the native tag manager
	const FWitchPTGameplayTags& WitchPtGameplayTags = FWitchPTGameplayTags::Get();
	
	TArray<FGameplayTag> PossibleInputs;
	PossibleInputs.Add(WitchPtGameplayTags.Ritual_Input_Up);
	PossibleInputs.Add(WitchPtGameplayTags.Ritual_Input_Down);
	PossibleInputs.Add(WitchPtGameplayTags.Ritual_Input_Left);
	PossibleInputs.Add(WitchPtGameplayTags.Ritual_Input_Right);
	
	// Generate random sequence
	for (int32 i = 0; i < SequenceLength; ++i)
	{
		int32 RandomIndex = FMath::RandRange(0, PossibleInputs.Num() - 1);
		InputSequence.Add(PossibleInputs[RandomIndex]);
	}
	
	// Reset sequence index
	Multicast_CurrentSequenceIndexChanged(CurrentSequenceIndex);
	
}

void ARitualAltar::HandlePlayerInput(ACharacter* Character, const FGameplayTag& InputTag)
{
	if (!Character || !HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("[DEBUG-RITUAL] Rejected input: no authority or invalid character"));
		return;
	}
	
	// Check if the ritual is active
	if (CurrentRitualState != EInteractionState::Active)
	{
		UE_LOG(LogTemp, Warning, TEXT("[DEBUG-RITUAL] Rejected input from %s: ritual not active (state=%d)"), 
			*Character->GetName(), static_cast<int32>(CurrentRitualState));
		return;
	}
	
	// Check if it's this player's turn
	if (Character != CurrentActivePlayer)
	{
		UE_LOG(LogTemp, Warning, TEXT("[DEBUG-RITUAL] Rejected input from %s: not their turn (current active=%s)"), 
			*Character->GetName(), CurrentActivePlayer ? *CurrentActivePlayer->GetName() : TEXT("None"));
		return;
	}
	
	// Check if we have a valid input to match against
	if (InputSequence.Num() == 0 || CurrentSequenceIndex >= InputSequence.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("[DEBUG-RITUAL] Input validation error: invalid sequence state (index=%d, sequence length=%d)"), 
			CurrentSequenceIndex, InputSequence.Num());
		return;
	}
	
	// Get the expected input for the current step
	FGameplayTag ExpectedInput = InputSequence[CurrentSequenceIndex];
	
	UE_LOG(LogTemp, Log, TEXT("[DEBUG-RITUAL] Input received from %s: %s (expected: %s)"), 
		*Character->GetName(), *InputTag.ToString(), *ExpectedInput.ToString());
	
	// Check if the input matches
	if (InputTag == ExpectedInput)
	{
		// Input successful
		UE_LOG(LogTemp, Log, TEXT("[DEBUG-RITUAL] Input CORRECT! Player: %s, Input: %s, Index: %d/%d"), 
			*Character->GetName(), *InputTag.ToString(), CurrentSequenceIndex, InputSequence.Num()-1);
		HandleInputSuccess(Character);
	
	}
	else
	{
		// Input failed
		UE_LOG(LogTemp, Warning, TEXT("[DEBUG-RITUAL] Input INCORRECT! Player: %s, Received: %s, Expected: %s"), 
			*Character->GetName(), *InputTag.ToString(), *ExpectedInput.ToString());
		HandleInputFailure(Character);
		
	}
}

void ARitualAltar::Multicast_OnInputSuccess_Implementation(ACharacter* Character)
{
	
	if (Character)
	{
		UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Character);
		if (ASC)
		{
			const FWitchPTGameplayTags& WitchPtGameplayTags = FWitchPTGameplayTags::Get();
			FGameplayEventData EventData;
			EventData.EventTag = WitchPtGameplayTags.Event_Ritual_InputSuccess;
			EventData.Instigator = this;
			EventData.Target = Character;
			
			// Buscar el tag de posición del jugador en el array
			FGameplayTag* PositionTag = nullptr;
			for (FPlayerPositionTagEntry& Entry : PlayerPositionTags)
			{
				if (Entry.Player == Character)
				{
					PositionTag = &Entry.PositionTag;
					break;
				}
			}
			if (PositionTag && PositionTag->MatchesTag(WitchPtGameplayTags.Get().Ritual_Position_1))
			{
				EventData.OptionalObject = PrimaryAnimMontage;
			} else
			{
				EventData.OptionalObject = SecondaryAnimMontage;
			}
			
			
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Character, WitchPtGameplayTags.Event_Ritual_InputSuccess, EventData);
		}
		
		
		UE_LOG(LogTemp, Log, TEXT("[RitualAltar] Input success feedback for player %s"), *Character->GetName());
	}
}

void ARitualAltar::HandleInputSuccess(ACharacter* Player)
{
	if (!HasAuthority() || !Player)
	{
		return;
	}
	
	// Cancel the input timer
	GetWorldTimerManager().ClearTimer(InputTimerHandle);
	
	// Advance to the next input
	CurrentSequenceIndex++;
	Multicast_CurrentSequenceIndexChanged(CurrentSequenceIndex);
	
	// Send success feedback
	
	Multicast_OnInputSuccess(Player);
	// Notify the player
	
	// Check if the sequence is complete
	if (CurrentSequenceIndex >= InputSequence.Num())
	{
		// Ritual succeeded
		CurrentRitualState = EInteractionState::Succeeded;
		
		
		// Spawn reward and notify
		SpawnReward();
		Multicast_OnRitualSucceeded();
		
		// Clean up
		CleanupRitual();
	}
	else
	{
		// Move to the next player's turn
		AdvanceToNextPlayer();
		
		// Restart the input timer
		StartInputTimer();
	}
}

void ARitualAltar::HandleInputFailure(ACharacter* Player)
{
	if (!HasAuthority() || !Player)
	{
		return;
	}
	
	// Cancel the input timer
	GetWorldTimerManager().ClearTimer(InputTimerHandle);
	
	// Increase corruption
	CorruptionAmount += CorruptionIncreasePerFail;
	// Advance to the next input
	CurrentSequenceIndex++;
	
	// Apply age penalty to the player
	ApplyAgePenalty(Player);
	
	// Send failure feedback
	Multicast_OnInputFailed(Player);
	
	// Check if corruption has reached the maximum
	if (CorruptionAmount >= MaxCorruption)
	{
		// Ritual failed catastrophically
		CurrentRitualState = EInteractionState::FailedCatastrophically;
		
		// Apply catastrophic penalties to all players
		for (ACharacter* ParticipatingPlayer : ParticipatingPlayers)
		{
			if (ParticipatingPlayer)
			{
				ApplyAgePenalty(ParticipatingPlayer, true);
			}
		}
		
		// Spawn demon and notify
		SpawnDemon();
		Multicast_OnRitualCatastrophicFail();
		
		// Clean up
		CleanupRitual();
	}
	else
	{
		// Move to the next player's turn
		AdvanceToNextPlayer();
		
		// Restart the input timer
		StartInputTimer();
	}
}

void ARitualAltar::StartInputTimer()
{
	if (!HasAuthority())
	{
		return;
	}
	
	// Calculate the time window for this input, possibly scaling down as the sequence progresses
	float ProgressDifficulty = FMath::Clamp(CurrentSequenceIndex / (float)InputSequence.Num(), 0.0f, 1.0f);
	float ScaledTimeWindow = BaseInputTimeWindow * (1.0f - (ProgressDifficulty * 0.5f)); // Up to 50% shorter at the end
	
	// Set the timer value for client display
	CurrentInputTimer = ScaledTimeWindow;
	
	
	// Start the timer
	GetWorldTimerManager().SetTimer(
		InputTimerHandle,
		this,
		&ARitualAltar::OnInputTimerExpired,
		ScaledTimeWindow,
		false
	);
	
	UE_LOG(LogTemp, Log, TEXT("[DEBUG-RITUAL] Started input timer: %.2f seconds for player %s, input %d/%d"), 
		ScaledTimeWindow, CurrentActivePlayer ? *CurrentActivePlayer->GetName() : TEXT("None"), 
		CurrentSequenceIndex, InputSequence.Num()-1);
	
	// Log the expected input
	if (InputSequence.IsValidIndex(CurrentSequenceIndex))
	{
		UE_LOG(LogTemp, Log, TEXT("[DEBUG-RITUAL] Current expected input: %s"), 
			*InputSequence[CurrentSequenceIndex].ToString());
	}
}

void ARitualAltar::OnInputTimerExpired()
{
	if (!HasAuthority() || CurrentRitualState != EInteractionState::Active)
	{
		return;
	}
	
	// If we get here, the player failed to input in time
	if (CurrentActivePlayer)
	{
		HandleInputFailure(CurrentActivePlayer);
	}
	else
	{
		// Fallback in case CurrentActivePlayer is null
		UE_LOG(LogTemp, Error, TEXT("[RitualAltar] Timer expired but no active player!"));
		AdvanceToNextPlayer();
		StartInputTimer();
	}
}

void ARitualAltar::AdvanceToNextPlayer()
{
	if (!HasAuthority() || ParticipatingPlayers.Num() == 0)
	{
		return;
	}
	
	
	// Find the index of the current active player
	int32 CurrentPlayerIndex = ParticipatingPlayers.IndexOfByKey(CurrentActivePlayer);
	
	// If not found or at the end, wrap around to the first player
	if (CurrentPlayerIndex == INDEX_NONE || CurrentPlayerIndex >= ParticipatingPlayers.Num() - 1)
	{
		CurrentPlayerIndex = 0;
	}
	else
	{
		CurrentPlayerIndex++;
	}
	
	// Safety check for eligible players
	int32 StartIndex = CurrentPlayerIndex;
	bool bFoundEligiblePlayer = false;
	
	// Loop through players starting from CurrentPlayerIndex until we find an eligible one
	do 
	{
		ACharacter* NextPlayer = ParticipatingPlayers[CurrentPlayerIndex];
		if (NextPlayer && IsPlayerEligibleForTurn(NextPlayer))
		{
			CurrentActivePlayer = NextPlayer;
			bFoundEligiblePlayer = true;
			FUIRitualData UIRitualData;
			
			UIRitualData.RitualPercentageCompleted = GetCurrentSequenceProgress();
			UIRitualData.CorruptionPercentage = GetCorruptionPercentage();
			if (CurrentActivePlayer->IsLocallyControlled() && CurrentActivePlayer->HasAuthority())
			{
				UIRitualData.bIsMyTurn = true;
				UIRitualData.ExpectedInput = InputSequence[CurrentSequenceIndex];
				OnIsMyTurnChangedDelegate.ExecuteIfBound(UIRitualData);
			} else 
			{
				UIRitualData.bIsMyTurn = false;
				UIRitualData.ExpectedInput = FGameplayTag::EmptyTag;
				OnIsMyTurnChangedDelegate.ExecuteIfBound(UIRitualData);
			}
			break;
		}
		
		// Move to next player
		CurrentPlayerIndex = (CurrentPlayerIndex + 1) % ParticipatingPlayers.Num();
	} 
	while (CurrentPlayerIndex != StartIndex && !bFoundEligiblePlayer);
	
	// If we couldn't find an eligible player, just use the first one as fallback
	if (!bFoundEligiblePlayer && ParticipatingPlayers.Num() > 0)
	{
		CurrentActivePlayer = ParticipatingPlayers[0];
	}
	
}

bool ARitualAltar::IsPlayerEligibleForTurn(ACharacter* Player) const
{
	if (!Player)
	{
		return false;
	}
	
	// Check if the player is still in a ritual position
	bool bIsInPosition = false;
	for (const ABaseInteractionPosition* Position : InteractionPositions)
	{
		if (Position && Position->IsOccupied() && Position->GetOccupyingCharacter() == Player)
		{
			bIsInPosition = true;
			break;
		}
	}
	
	// Check if the player has the OccupyingPosition tag
	const FWitchPTGameplayTags& WitchPtGameplayTags = FWitchPTGameplayTags::Get();
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Player);
	bool bHasPositionTag = ASC && ASC->HasMatchingGameplayTag(WitchPtGameplayTags.Character_State_Ritual_InPosition);
	
	return bIsInPosition && bHasPositionTag;
}

void ARitualAltar::ApplyAgePenalty(ACharacter* Player, bool bCatastrophic)
{
	if (!HasAuthority() || !Player)
	{
		return;
	}
	
	// Get the player's ASC
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Player);
	if (!ASC)
	{
		return;
	}
	
	// TODO: Apply aging GameplayEffect
	// This would typically be implemented using a GameplayEffect that increases age
	// The effect class would be defined elsewhere and referenced here
	
	// Pseudo-code:
	// TSubclassOf<UGameplayEffect> AgeEffect = bCatastrophic ? CatastrophicAgeEffectClass : StandardAgeEffectClass;
	// if (AgeEffect)
	// {
	//     FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
	//     ContextHandle.AddSourceObject(this);
	//     FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(AgeEffect, 1.0f, ContextHandle);
	//     ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	// }
	
	UE_LOG(LogTemp, Log, TEXT("[RitualAltar] Applied %s age penalty to %s"), 
		bCatastrophic ? TEXT("catastrophic") : TEXT("standard"), *Player->GetName());
}

void ARitualAltar::SpawnReward()
{
	if (!HasAuthority())
	{
		return;
	}
	
	// TODO: Implement reward spawning logic
	// This would typically create a special item and add it to inventory
	
	UE_LOG(LogTemp, Log, TEXT("[RitualAltar] Spawned ritual reward"));
}

void ARitualAltar::SpawnDemon()
{
	if (!HasAuthority())
	{
		return;
	}
	
	// TODO: Implement demon spawning logic
	// This would typically spawn an enemy character
	
	UE_LOG(LogTemp, Log, TEXT("[RitualAltar] Spawned demon"));
}

void ARitualAltar::CleanupRitual()
{
	if (!HasAuthority())
	{
		return;
	}
	
	// Clear timers
	GetWorldTimerManager().ClearTimer(InputTimerHandle);
	
	// Reset ritual-specific variables
	CurrentSequenceIndex = 0;
	InputSequence.Empty();
	CurrentActivePlayer = nullptr;
	
	// Don't reset corruption or state - these should persist for UI feedback
	
	// Reset positions? This depends on design - maybe players stay in position
	
	UE_LOG(LogTemp, Log, TEXT("[RitualAltar] Ritual cleaned up"));
}

float ARitualAltar::GetCurrentSequenceProgress() const
{
	if (InputSequence.Num() == 0)
	{
		return 0.0f;
	}
	
	return static_cast<float>(CurrentSequenceIndex) / static_cast<float>(InputSequence.Num());
}

FGameplayTag ARitualAltar::GetCurrentExpectedInput() const
{
	if (InputSequence.Num() == 0 || CurrentSequenceIndex >= InputSequence.Num())
	{
		
		return FGameplayTag();
	}
	
	return InputSequence[CurrentSequenceIndex];
}



FGameplayTag ARitualAltar::ConvertERitualInputToTag(ERitualInput Input)
{
	const FWitchPTGameplayTags& WitchPtGameplayTags = FWitchPTGameplayTags::Get();
	
	switch (Input)
	{
	case ERitualInput::Up:
		return WitchPtGameplayTags.Ritual_Input_Up;
	case ERitualInput::Down:
		return WitchPtGameplayTags.Ritual_Input_Down;
	case ERitualInput::Left:
		return WitchPtGameplayTags.Ritual_Input_Left;
	case ERitualInput::Right:
		return WitchPtGameplayTags.Ritual_Input_Right;
	default:
		return FGameplayTag();
	}
}

ERitualInput ARitualAltar::ConvertTagToERitualInput(const FGameplayTag& Tag)
{
	const FWitchPTGameplayTags& WitchPtGameplayTags = FWitchPTGameplayTags::Get();
	
	if (Tag == WitchPtGameplayTags.Ritual_Input_Up)
		return ERitualInput::Up;
	if (Tag == WitchPtGameplayTags.Ritual_Input_Down)
		return ERitualInput::Down;
	if (Tag == WitchPtGameplayTags.Ritual_Input_Left)
		return ERitualInput::Left;
	if (Tag == WitchPtGameplayTags.Ritual_Input_Right)
		return ERitualInput::Right;
	
	return ERitualInput::None;
}

void ARitualAltar::Multicast_OnInputFailed_Implementation(ACharacter* Character)
{
	if (Character)
	{
		UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Character);
		if (ASC)
		{
			const FWitchPTGameplayTags& WitchPtGameplayTags = FWitchPTGameplayTags::Get();
			FGameplayEventData EventData;
			EventData.EventTag = WitchPtGameplayTags.Event_Ritual_InputSuccess;
			EventData.Instigator = this;
			EventData.Target = Character;
			
			EventData.OptionalObject = FailedAnimMontage;
			
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Character, WitchPtGameplayTags.Event_Ritual_InputFailure, EventData);
			
		}
		
		
		
		UE_LOG(LogTemp, Log, TEXT("[RitualAltar] Input success feedback for player %s"), *Character->GetName());
	}
	
	
}



void ARitualAltar::OccupyPosition(ACharacter* Player, ABaseInteractionPosition* Position)
{
	// Print the local role por the RitualAltar and the Player
	UE_LOG(LogTemp, Warning, TEXT("[RitualAltar] Player LocalRole Role: %s"), *UEnum::GetValueAsString(Player->GetLocalRole()));
	UE_LOG(LogTemp, Warning, TEXT("[RitualAltar] Player RemoteRole Role: %s"), *UEnum::GetValueAsString(Player->GetRemoteRole()));
	if (Player->IsLocallyControlled())
	{
		UE_LOG(LogTemp, Warning, TEXT("[RitualAltar] Player is locally controlled"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[RitualAltar] Player is NOT locally controlled"));
	}
	
	
	Super::OccupyPosition(Player, Position);
	CurrentRitualState = EInteractionState::WaitingForPlayers;
	
	

	// Call the ritual state delegate for Listen Server
	if (Player->GetLocalRole() == ROLE_Authority && Player->IsLocallyControlled()) // Im the listen server
	{
		AWitchPTPlayerController* PC = Cast<AWitchPTPlayerController>(Player->GetOwner());
		if (!PC->HasRitualWidgetInitialized(this))
		{
			PC->LocalInitializeRitualUserWidget(this);
			OnRitualStateChangedDelegate.ExecuteIfBound(CurrentRitualState);
		}
	} else if (Player->HasAuthority() && !Player->IsLocallyControlled()) // The call is from the client
	{
		AWitchPTPlayerController* PC = Cast<AWitchPTPlayerController>(Player->GetOwner());
		if (!PC->HasRitualWidgetInitialized(this))
		{
			PC->Client_InitializeRitualUserWidget(this);
		}
		
	}
	
	
	Multicast_NumberOfPlayersReadyHasChanged(ParticipatingPlayers.Num(), ReadyPlayers.Num());
}

void ARitualAltar::Multicast_OnRitualSucceeded_Implementation()
{
	// Client-side feedback for ritual success
	// This would typically play sounds, particle effects, etc.
	
	// Example: Play celebratory effects at altar location
	// UGameplayStatics::PlaySoundAtLocation(this, SuccessSound, GetActorLocation());
	// UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SuccessParticles, GetActorTransform());
	OnRitualCompletedDelegate.ExecuteIfBound(true);
	DestroyAltarPositions();
	
	UE_LOG(LogTemp, Log, TEXT("[RitualAltar] Ritual succeeded feedback"));
}

void ARitualAltar::Multicast_OnRitualCatastrophicFail_Implementation()
{
	// Client-side feedback for catastrophic failure
	// This would typically play sounds, particle effects, etc.
	
	// Example: Play ominous effects at altar location
	// UGameplayStatics::PlaySoundAtLocation(this, CatastrophicFailSound, GetActorLocation());
	// UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CatastrophicFailParticles, GetActorTransform());
	
	UE_LOG(LogTemp, Log, TEXT("[RitualAltar] Ritual catastrophically failed feedback"));
	OnRitualCompletedDelegate.ExecuteIfBound(false);
}


void ARitualAltar::OnRep_CurrentRitualState(EInteractionState NewState)
{
	OnRitualStateChangedDelegate.ExecuteIfBound(CurrentRitualState);
}

void ARitualAltar::OnRep_CurrentActivePlayer(const ACharacter* NewActivePlayer)
{
	// if (CurrentActivePlayer == nullptr) return;
	
}

void ARitualAltar::OnRep_CurrentSequenceIndex(int32 NewSequenceIndex)
{
	if (CurrentActivePlayer == nullptr) return;
	
	FUIRitualData UIRitualData;
	UIRitualData.RitualPercentageCompleted = GetCurrentSequenceProgress();
	UIRitualData.CorruptionPercentage = GetCorruptionPercentage();
	if (CurrentActivePlayer->IsLocallyControlled())
	{
		UIRitualData.bIsMyTurn = true;
		UIRitualData.ExpectedInput = InputSequence[CurrentSequenceIndex];
		OnIsMyTurnChangedDelegate.ExecuteIfBound(UIRitualData);
	} else
	{
		UIRitualData.bIsMyTurn = false;
		UIRitualData.ExpectedInput = FGameplayTag::EmptyTag;
		OnIsMyTurnChangedDelegate.ExecuteIfBound(UIRitualData);
	}
}


float ARitualAltar::GetCorruptionPercentage() const
{
	if (MaxCorruption == 0.0f)
	{
		return 0.0f;
	}
	
	return CorruptionAmount / MaxCorruption;
}

void ARitualAltar::OnRep_CorruptionAmount(float NewCorruptionAmount)
{
	OnCorruptionAmountChangedDelegate.ExecuteIfBound(NewCorruptionAmount);
}

