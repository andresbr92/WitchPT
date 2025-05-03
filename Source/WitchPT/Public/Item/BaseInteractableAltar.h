// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h" // Include for FGameplayTag
#include "BaseInteractableAltar.generated.h"

class ABaseInteractionPosition;
class ACharacter;

/**
 * Base state enum for interaction state
 */
UENUM(BlueprintType)
enum class EInteractionState : uint8
{
    Inactive    UMETA(DisplayName = "Inactive"),
    Preparing   UMETA(DisplayName = "Preparing"), // Players occupying positions
    Active      UMETA(DisplayName = "Active"),    // Sequence running
    Succeeded   UMETA(DisplayName = "Succeeded"),
    Failed      UMETA(DisplayName = "Failed"),
    FailedCatastrophically UMETA(DisplayName = "Failed Catastrophically")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractionCompleted, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInputReceived, ACharacter*, Character, bool, bWasCorrect);

/**
 * Base class for interactive altars/cauldrons that players can interact with
 */
UCLASS(Abstract)
class WITCHPT_API ABaseInteractableAltar : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ABaseInteractableAltar();
    virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
    
    // Current interaction state
    UPROPERTY(ReplicatedUsing = OnRep_CurrentState, BlueprintReadOnly, Category = "Interaction|State")
    EInteractionState CurrentState = EInteractionState::Inactive;
    
    // List of players participating in the interaction
    UPROPERTY(ReplicatedUsing = OnRep_ParticipatingPlayers, BlueprintReadWrite, VisibleAnywhere, Category = "Interaction")
    TArray<TObjectPtr<ACharacter>> ParticipatingPlayers;
    
    
    // Positions for interaction
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|Setup")
    TArray<TObjectPtr<ABaseInteractionPosition>> InteractionPositions;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interaction|Setup")
    TMap<TObjectPtr<ACharacter>, FGameplayTag> PlayerPositionTags;
    
    // Basic interaction methods
    UFUNCTION(BlueprintCallable)
    virtual void OccupyPosition(ACharacter* Player, ABaseInteractionPosition* Position);
    
    // Multicast RPCs for notifications
    UFUNCTION(NetMulticast, Reliable)
    void Multicast_OnStateChanged(EInteractionState NewState);
    
    UFUNCTION(NetMulticast, Reliable)
    virtual void Multicast_OnInputSuccess(ACharacter* Character);
    
    UFUNCTION(NetMulticast, Reliable)
    virtual void Multicast_OnInputFailed(ACharacter* Character);
    
    UFUNCTION(NetMulticast, Reliable)
    void Multicast_OnInteractionSucceeded();
    
    UFUNCTION(NetMulticast, Reliable)
    void Multicast_OnInteractionCatastrophicFail();
    
    // Getters for Blueprint/HUD access
    UFUNCTION(BlueprintPure, Category = "Interaction")
    EInteractionState GetCurrentState() const { return CurrentState; }
    
    // Delegates
    UPROPERTY(BlueprintAssignable, Category = "Interaction")
    FOnInteractionCompleted OnInteractionCompleted;
    
    UPROPERTY(BlueprintAssignable, Category = "Interaction")
    FOnInputReceived OnInputReceived;

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    
    // OnRep functions for replicated properties
    UFUNCTION()
    virtual void OnRep_CurrentState();
    
    UFUNCTION()
    virtual void OnRep_ParticipatingPlayers();
    
    
    // Helper functions
    virtual void HandleInputSuccess(ACharacter* Player);
    virtual void HandleInputFailure(ACharacter* Player);
    virtual void ApplyAgePenalty(ACharacter* Player, bool bCatastrophic = false);
    virtual void CheckState();
    virtual void CleanupInteraction();
    virtual void SpawnReward();
    virtual void SpawnDemon();
    virtual bool IsPlayerEligibleForInteraction(ACharacter* Player) const;
    
}; 