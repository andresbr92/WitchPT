// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/BaseInteractableAltar.h"
#include "GameplayTagContainer.h" // Include for FGameplayTag
#include "RitualAltar.generated.h"

class URitualUserWidget;
class ARitualPosition;

// Enum defining the possible inputs for the ritual sequence
UENUM(BlueprintType)
enum class ERitualInput : uint8
{
	Up		UMETA(DisplayName = "Up"),
	Down	UMETA(DisplayName = "Down"),
	Left	UMETA(DisplayName = "Left"),
	Right	UMETA(DisplayName = "Right"),
	None	UMETA(DisplayName = "None") // Optional: For default/invalid state
};


DECLARE_DELEGATE_TwoParams(FOnReadyPlayersChanged, int32 TotalPlayers, int32 PlayersReady);
DECLARE_DELEGATE_OneParam(FOnCurrentActivePlayerChanged, const ACharacter* Character);

UCLASS()
class WITCHPT_API ARitualAltar : public ABaseInteractableAltar
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARitualAltar();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	// ----------------------------------- PROPERTIES ---------------------------------------------- //
	// Current ritual state
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Ritual|State")
	EInteractionState CurrentRitualState = EInteractionState::Inactive;
	
	// Current sequence of inputs required for the ritual
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Ritual")
	TArray<FGameplayTag> InputSequence;

	// Current index in the sequence
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Ritual")
	int32 CurrentSequenceIndex = 0;
	
	// Players who have confirmed they're ready to start
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Ritual|State")
	TArray<TObjectPtr<ACharacter>> ReadyPlayers;
	
	// Current countdown value (when starting ritual)
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Ritual|State")
	int32 StartCountdown = 3;
	
	// Current player whose turn it is to input
	UPROPERTY(ReplicatedUsing = OnRep_CurrentActivePlayer, BlueprintReadOnly, Category = "Ritual|State")
	TObjectPtr<ACharacter> CurrentActivePlayer;
	
	// Timer for the current input
	UPROPERTY(Replicated, BlueprintReadWrite, VisibleAnywhere, Category = "Ritual")
	float CurrentInputTimer;
	
	// Current corruption level
	UPROPERTY(Replicated, BlueprintReadWrite, VisibleAnywhere, Category = "Ritual")
	float CorruptionAmount = 0.0f;

	// Maximum corruption allowed before catastrophic failure
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category = "Ritual")
	float MaxCorruption = 100.0f;
	
	// Corruption increase per failure
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category = "Ritual")
	float CorruptionIncreasePerFail = 10.0f;
	
	// Base time window for inputs
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category = "Ritual")
	float BaseInputTimeWindow = 10.0f;
	
	// Scaling multiplier for difficulty
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category = "Ritual")
	float DifficultyScalingMultiplier = 1.0f;
	// ----------------------------------- DELEGATES ---------------------------------------------- //
	FOnReadyPlayersChanged OnReadyPlayersNumberChangedDelegate;
	FOnCurrentActivePlayerChanged OnCurrentActivePlayerChanged;

	// ----------------------------------- MAIN FUNCTIONS ---------------------------------------------- //
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayersReadyNumberChanged(int32 TotalPlayers, int32 PlayersReady);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_CurrentActivePlayerChanged(const ACharacter* Character);
	
	
	// ----------------------------------- MAIN FUNCTIONS ---------------------------------------------- //
	void StartRitual(ACharacter* RequestingCharacter);
	void HandlePlayerInput(ACharacter* Character, const FGameplayTag& InputTag);
	
	// New function to check player ready status
	UFUNCTION(BlueprintPure, Category = "Ritual")
	bool IsPlayerReady(ACharacter* Player) const;
	
	// New function to get the percentage of ready players
	UFUNCTION(BlueprintPure, Category = "Ritual")
	float GetReadyPlayersPercentage() const;
	
	// New function to check if all players are ready
	UFUNCTION(BlueprintPure, Category = "Ritual")
	bool AreAllPlayersReady() const;
	
	virtual void Multicast_OnInputSuccess_Implementation(ACharacter* Character) override;
	virtual void Multicast_OnInputFailed_Implementation(ACharacter* Character) override;
	
	// Multicast RPCs for notifications
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnRitualStateChanged(EInteractionState RitualState);
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnRitualSucceeded();
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnRitualCatastrophicFail();
	
	
	// New multicast RPC for countdown
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnCountdownTick(int32 CountdownValue);

	// --------------- ON REPS FUNCTIONS -----------------//
	UFUNCTION()
	void OnRep_CurrentActivePlayer();
	
	
	virtual void OccupyPosition(ACharacter* Player, ABaseInteractionPosition* Position) override;
	
	// Getters for Blueprint/HUD access
	UFUNCTION(BlueprintPure, Category = "Ritual")
	EInteractionState GetCurrentRitualState() const { return CurrentRitualState; }
	
	UFUNCTION(BlueprintPure, Category = "Ritual")
	ACharacter* GetCurrentActivePlayer() const { return CurrentActivePlayer; }
	
	UFUNCTION(BlueprintPure, Category = "Ritual")
	float GetCorruptionPercentage() const { return MaxCorruption > 0.0f ? (CorruptionAmount / MaxCorruption) * 100.0f : 0.0f; }
	
	UFUNCTION(BlueprintPure, Category = "Ritual")
	float GetCurrentInputTimeRemaining() const { return CurrentInputTimer; }
	
	UFUNCTION(BlueprintPure, Category = "Ritual")
	int32 GetCurrentSequenceProgress() const { return InputSequence.Num() > 0 ? (CurrentSequenceIndex * 100) / InputSequence.Num() : 0; }
	
	UFUNCTION(BlueprintPure, Category = "Ritual")
	FGameplayTag GetCurrentExpectedInput() const;

	

	// Delegate specifically for ritual inputs
	UPROPERTY(BlueprintAssignable, Category = "Ritual")
	FOnInputReceived OnRitualInputReceived;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URitualUserWidget> RitualUserWidgetClass;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	// Timer handle for the ritual start countdown
	FTimerHandle RitualStartCountdownHandle;
	
	// Timer handles
	FTimerHandle InputTimerHandle;
	
	
	// Helper functions
	void GenerateInputSequence();
	void AdvanceToNextPlayer();
	virtual void HandleInputSuccess(ACharacter* Player) override;
	virtual void HandleInputFailure(ACharacter* Player) override;
	void ApplyAgePenalty(ACharacter* Player, bool bCatastrophic = false);
	void StartInputTimer();
	void OnInputTimerExpired();
	void CheckRitualState();
	FGameplayTag ConvertERitualInputToTag(ERitualInput Input);
	ERitualInput ConvertTagToERitualInput(const FGameplayTag& Tag);
	void CleanupRitual();
	void SpawnReward();
	void SpawnDemon();
	bool IsPlayerEligibleForTurn(ACharacter* Player) const;
	
	// New helper functions for the ready system
	void ProcessRitualReadyRequest(ACharacter* RequestingCharacter);
	void StartRitualCountdown();
	void ProcessCountdownTick();
	void ActivateRitual();

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> PrimaryAnimMontage;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> SecondaryAnimMontage;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> FailedAnimMontage;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> FailedCatastrophicallyAnimMontage;
	
	// Animation montages specifically for ritual
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> RitualWaitingAnimMontage;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> RitualActiveAnimMontage;
};
