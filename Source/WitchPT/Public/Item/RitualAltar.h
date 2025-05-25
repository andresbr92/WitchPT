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

USTRUCT(BlueprintType)
struct FUIRitualData
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadOnly)
	bool bIsMyTurn = false;
	UPROPERTY(BlueprintReadOnly)
	FGameplayTag ExpectedInput = FGameplayTag::EmptyTag;
	UPROPERTY(BlueprintReadOnly)
	float RitualPercentageCompleted = 0.0f;
	UPROPERTY(BlueprintReadOnly)
	float CorruptionPercentage = 0.0f;
	UPROPERTY(BlueprintReadOnly)
	float CurrentInputTimeRemaining = 0.0f;
};

// Replicated struct for ready players data
USTRUCT(BlueprintType)
struct FRitualReadyPlayersData
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly)
	int32 TotalPlayers = 0;
	
	UPROPERTY(BlueprintReadOnly)
	int32 ReadyPlayers = 0;
	
	bool operator==(const FRitualReadyPlayersData& Other) const
	{
		return TotalPlayers == Other.TotalPlayers && ReadyPlayers == Other.ReadyPlayers;
	}
	
	bool operator!=(const FRitualReadyPlayersData& Other) const
	{
		return !(*this == Other);
	}
};

// Delegates for UI updates - these will be called from OnRep functions
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRitualStateChanged, EInteractionState, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReadyPlayersChanged, FRitualReadyPlayersData, ReadyPlayersData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCountdownTick, int32, CountdownValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTurnDataChanged, FUIRitualData, TurnData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCorruptionChanged, float, CorruptionPercentage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSequenceProgressChanged, float, ProgressPercentage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRitualCompleted, bool, bWasSuccessful);

UCLASS()
class WITCHPT_API ARitualAltar : public ABaseInteractableAltar
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARitualAltar();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	// ----------------------------------- REPLICATED PROPERTIES ---------------------------------------------- //
	// Current ritual state
	UPROPERTY(ReplicatedUsing = OnRep_CurrentRitualState, Category = "Ritual|State", VisibleAnywhere)
	EInteractionState CurrentRitualState = EInteractionState::Inactive;
	
	// Current sequence of inputs required for the ritual
	UPROPERTY(Replicated, Category = "Ritual", VisibleAnywhere)
	TArray<FGameplayTag> InputSequence;

	// Current index in the sequence
	UPROPERTY(ReplicatedUsing = OnRep_CurrentSequenceIndex, Category = "Ritual", VisibleAnywhere)
	int32 CurrentSequenceIndex = -1;
	
	// Players who have confirmed they're ready to start
	UPROPERTY(Replicated, Category = "Ritual|State", VisibleAnywhere)
	TArray<TObjectPtr<ACharacter>> ReadyPlayers;
	
	// Ready players data for UI
	UPROPERTY(ReplicatedUsing = OnRep_ReadyPlayersData, Category = "Ritual|State", VisibleAnywhere)
	FRitualReadyPlayersData ReadyPlayersData;
	
	// Current countdown value (when starting ritual)
	UPROPERTY(ReplicatedUsing = OnRep_StartCountdown, Category = "Ritual|State", VisibleAnywhere)
	int32 StartCountdown = 3;
	
	// Current player whose turn it is to input
	UPROPERTY(ReplicatedUsing = OnRep_CurrentActivePlayer, VisibleAnywhere, Category = "Ritual|State")
	TObjectPtr<ACharacter> CurrentActivePlayer;
	
	// Timer for the current input
	UPROPERTY(Replicated, VisibleAnywhere, Category = "Ritual")
	float CurrentInputTimer;
	
	// Current turn data for UI
	UPROPERTY(ReplicatedUsing = OnRep_TurnData, VisibleAnywhere, Category = "Ritual|UI")
	FUIRitualData CurrentTurnData;
	
	// Current corruption level
	UPROPERTY(ReplicatedUsing = OnRep_CorruptionAmount, VisibleAnywhere, Category = "Ritual")
	float CorruptionAmount = 0.0f;

	// Maximum corruption allowed before catastrophic failure
	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Ritual|Configuration")
	float MaxCorruption = 100.0f;
	
	// Corruption increase per failure
	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Ritual|Configuration")
	float CorruptionIncreasePerFail = 10.0f;
	
	// Base time window for inputs
	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Ritual|Configuration")
	float BaseInputTimeWindow = 10.0f;
	
	// Scaling multiplier for difficulty
	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Ritual|Configuration")
	float DifficultyScalingMultiplier = 1.0f;
	
	// Ritual completion status
	UPROPERTY(ReplicatedUsing = OnRep_RitualCompleted, VisibleAnywhere, Category = "Ritual|State")
	bool bRitualCompleted = false;
	
	UPROPERTY(Replicated, VisibleAnywhere, Category = "Ritual|State")
	bool bRitualWasSuccessful = false;
	
	// ----------------------------------- DELEGATES ---------------------------------------------- //
	FOnRitualStateChanged OnRitualStateChangedEvent;
	
	FOnReadyPlayersChanged OnReadyPlayersChangedEvent;
	
	FOnCountdownTick OnCountdownTickEvent;
	
	FOnTurnDataChanged OnTurnDataChangedEvent;
	
	FOnCorruptionChanged OnCorruptionChangedEvent;
	
	FOnSequenceProgressChanged OnSequenceProgressChangedEvent;
	UPROPERTY(BlueprintAssignable)
	FOnRitualCompleted OnRitualCompletedEvent;

	// ----------------------------------- ONREP FUNCTIONS ---------------------------------------------- //
	UFUNCTION()
	void OnRep_CurrentRitualState();
	
	UFUNCTION()
	void OnRep_CurrentSequenceIndex();
	
	UFUNCTION()
	void OnRep_ReadyPlayersData();
	
	UFUNCTION()
	void OnRep_StartCountdown();
	
	UFUNCTION()
	void OnRep_CurrentActivePlayer();
	
	UFUNCTION()
	void OnRep_TurnData();
	
	UFUNCTION()
	void OnRep_CorruptionAmount();
	
	UFUNCTION()
	void OnRep_RitualCompleted();

	// ----------------------------------- MAIN FUNCTIONS ---------------------------------------------- //
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_NumberOfPlayersReadyHasChanged(int32 TotalPlayers, int32 PlayersReady);
	
	void StartRitual(ACharacter* RequestingCharacter);
	void HandlePlayerInput(ACharacter* Character, const FGameplayTag& InputTag);
	
	// New function to check player ready status
	UFUNCTION(BlueprintPure, Category = "Ritual")
	bool IsPlayerReady(ACharacter* Player) const;
	
	// New function to check if all players are ready
	UFUNCTION(BlueprintPure, Category = "Ritual")
	bool AreAllPlayersReady() const;
	
	virtual void Multicast_OnInputSuccess_Implementation(ACharacter* Character) override;
	virtual void Multicast_OnInputFailed_Implementation(ACharacter* Character) override;
	
	// Multicast RPCs for notifications
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnRitualSucceeded();
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnRitualCatastrophicFail();
	
	// New multicast RPC for countdown
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnCountdownTick(int32 CountdownValue);
	
	virtual void OccupyPosition(ACharacter* Player, ABaseInteractionPosition* Position) override;
	
	// ---------------------------- GETTERS FOR WC ---------------------------- //
	EInteractionState GetCurrentRitualState() const { return CurrentRitualState; }
	ACharacter* GetCurrentActivePlayer() const { return CurrentActivePlayer; }
	float GetCorruptionPercentage() const;
	float GetCurrentInputTimeRemaining() const { return CurrentInputTimer; }
	float GetCurrentSequenceProgress() const;
	FGameplayTag GetCurrentExpectedInput() const;
	int32 GetNumberOfReadyPlayers() const { return ReadyPlayers.Num(); }
	int32 GetNumberOfTotalPlayers() const { return ParticipatingPlayers.Num(); }
	FUIRitualData GetCurrentTurnData() const { return CurrentTurnData; }


protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	// Timer handle for the ritual start countdown
	FTimerHandle RitualStartCountdownHandle;
	
	// Timer handles
	FTimerHandle InputTimerHandle;
	
	// ----------------------------------- LOGIC FUNCTIONS ---------------------------------------------- //
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
	void UpdateTurnData();
	void UpdateReadyPlayersData();
	
	// New helper functions for the ready system
	void ProcessRitualReadyRequest(ACharacter* RequestingCharacter);
	void StartRitualCountdown();
	void ProcessCountdownTick();
	void ActivateRitual();
	
	// ----------------------------------- BROADCAST HELPER FUNCTIONS ---------------------------------------------- //
	// These functions handle event broadcasting and are called both from server-side code and OnRep functions
	void BroadcastRitualStateChanged();
	void BroadcastReadyPlayersChanged();
	void BroadcastCountdownTick();
	void BroadcastTurnDataChanged();
	void BroadcastCorruptionChanged();
	void BroadcastSequenceProgressChanged();
	void BroadcastRitualCompleted();

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
