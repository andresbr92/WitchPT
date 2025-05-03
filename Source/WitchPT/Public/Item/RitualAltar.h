// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/BaseInteractableAltar.h"
#include "GameplayTagContainer.h" // Include for FGameplayTag
#include "RitualAltar.generated.h"

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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSequenceCompleted, bool, bWasSuccessful);
UCLASS()
class WITCHPT_API ARitualAltar : public ABaseInteractableAltar
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARitualAltar();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	// Current ritual state
	UPROPERTY(ReplicatedUsing = OnRep_CurrentRitualState, BlueprintReadOnly, Category = "Ritual|State")
	EInteractionState CurrentRitualState = EInteractionState::Inactive;
	
	// Current sequence of inputs required for the ritual
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Ritual")
	TArray<FGameplayTag> InputSequence;

	// Current index in the sequence
	UPROPERTY(ReplicatedUsing = OnRep_CurrentSequenceIndex, BlueprintReadOnly, Category = "Ritual")
	int32 CurrentSequenceIndex = 0;
	
	
	// Current player whose turn it is to input
	UPROPERTY(ReplicatedUsing = OnRep_CurrentActivePlayer, BlueprintReadOnly, Category = "Ritual|State")
	TObjectPtr<ACharacter> CurrentActivePlayer;
	
	// Timer for the current input
	UPROPERTY(ReplicatedUsing = OnRep_CurrentInputTimer, BlueprintReadWrite, VisibleAnywhere, Category = "Ritual")
	float CurrentInputTimer;
	
	// Current corruption level
	UPROPERTY(ReplicatedUsing = OnRep_CorruptionAmount, BlueprintReadWrite, VisibleAnywhere, Category = "Ritual")
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
	
	// Positions for the ritual
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ritual|Setup")
	TArray<TObjectPtr<ARitualPosition>> RitualPositions;

	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnSequenceCompleted OnSequenceCompleted;
	
	
	// RPCs for client-server communication
	UFUNCTION(BlueprintCallable)
	void StartRitual(ACharacter* InitiatingPlayer);
	
	UFUNCTION(BlueprintCallable)
	bool HandlePlayerInput(ACharacter* Character, const FGameplayTag& InputTag);
	
	
	// Multicast RPCs for notifications
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnRitualStateChanged(EInteractionState NewState);
	
	
	virtual void Multicast_OnInputSuccess_Implementation(ACharacter* Character) override;
	
	virtual void Multicast_OnInputFailed_Implementation(ACharacter* Character) override;
	
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnRitualSucceeded();
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnRitualCatastrophicFail();

	

	
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

	// Delegates
	


	// Delegate specifically for ritual inputs
	UPROPERTY(BlueprintAssignable, Category = "Ritual")
	FOnInputReceived OnRitualInputReceived;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	// OnRep functions for replicated properties
	UFUNCTION()
	void OnRep_CurrentRitualState();
	
	UFUNCTION()
	void OnRep_CurrentSequenceIndex();
	
	
	
	UFUNCTION()
	void OnRep_CurrentActivePlayer();
	
	UFUNCTION()
	void OnRep_CurrentInputTimer();
	
	UFUNCTION()
	void OnRep_CorruptionAmount();
	
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
