// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/BaseInteractableAltar.h"
#include "GameplayTagContainer.h" // Include for FGameplayTag
#include "CauldronAltar.generated.h"

class ACauldronPosition;
class ACharacter;

// Estructura para contener secuencias de ingredientes
USTRUCT(BlueprintType)
struct FIngredientSequence
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    TArray<FGameplayTag> Ingredients;
};

// Enum for cauldron inputs
UENUM(BlueprintType)
enum class ECauldronInput : uint8
{
    Ingredient1 UMETA(DisplayName = "Ingredient 1"),
    Ingredient2 UMETA(DisplayName = "Ingredient 2"),
    Ingredient3 UMETA(DisplayName = "Ingredient 3"),
    Ingredient4 UMETA(DisplayName = "Ingredient 4"),
    Ingredient5 UMETA(DisplayName = "Ingredient 5"),
    None        UMETA(DisplayName = "None")
};

UENUM(BlueprintType)
enum ECauldronPhysicState
{
    Static UMETA(DisplayName = "InPlace"),
    Moving UMETA(DisplayName = "Moving")
};

/**
 * Cauldron altar allows players to add ingredients in any order (unlike ritual's sequential inputs)
 */
UCLASS()
class WITCHPT_API ACauldronAltar : public ABaseInteractableAltar
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ACauldronAltar();
    virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
    
    // Map of player to their assigned input sequence
    UPROPERTY(BlueprintReadOnly, Category = "Cauldron")
    TMap<TObjectPtr<ACharacter>, FIngredientSequence> PlayerInputSequences;
    
    // Map of player to their current sequence progress
    UPROPERTY(BlueprintReadOnly, Category = "Cauldron")
    TMap<TObjectPtr<ACharacter>, int32> PlayerSequenceIndices;
    
    // Time window for adding ingredients
    UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category = "Cauldron")
    float IngredientTimeWindow = 30.0f;
    
    // Timer for the overall cauldron brewing
    UPROPERTY(ReplicatedUsing = OnRep_CauldronTimer, BlueprintReadWrite, VisibleAnywhere, Category = "Cauldron")
    float CauldronTimer;

    UPROPERTY(ReplicatedUsing = OnRep_CauldronTimer, BlueprintReadWrite, VisibleAnywhere, Category = "Cauldron")
    TEnumAsByte<ECauldronPhysicState> CauldronPhysicState;
    
    // Positions for the cauldron (override the base class property with the correct type)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cauldron|Setup")
    TArray<TObjectPtr<ACauldronPosition>> CauldronPositions;


    
    
    // RPCs for client-server communication
    UFUNCTION(BlueprintCallable)
    void StartBrewing(ACharacter* InitiatingPlayer);
    
    UFUNCTION(BlueprintCallable)
    bool HandlePlayerInput(ACharacter* Character, const FGameplayTag& InputTag);
    
    // Getters for Blueprint/HUD access
    UFUNCTION(BlueprintPure, Category = "Cauldron")
    float GetCauldronTimeRemaining() const { return CauldronTimer; }
    
    UFUNCTION(BlueprintPure, Category = "Cauldron")
    int32 GetPlayerSequenceProgress(ACharacter* Player) const;
    
    UFUNCTION(BlueprintPure, Category = "Cauldron")
    TArray<FGameplayTag> GetPlayerSequence(ACharacter* Player) const;
    
    UFUNCTION(BlueprintPure, Category = "Cauldron")
    FGameplayTag GetCurrentExpectedInputForPlayer(ACharacter* Player) const;

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    
    // OnRep functions for replicated properties
    UFUNCTION()
    void OnRep_CauldronTimer();
    
    // Timer handles
    FTimerHandle CauldronTimerHandle;
    
    // Helper functions
    void GeneratePlayerSequences();
    virtual void HandleInputSuccess(ACharacter* Player) override;
    virtual void HandleInputFailure(ACharacter* Player) override;
    void StartCauldronTimer();
    void OnCauldronTimerExpired();
    FGameplayTag ConvertECauldronInputToTag(ECauldronInput Input);
    ECauldronInput ConvertTagToECauldronInput(const FGameplayTag& Tag);
    virtual bool IsPlayerEligibleForInteraction(ACharacter* Player) const override;
    bool AreAllPlayersDone() const;
    
    // Animation montages
    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UAnimMontage> BrewingAnimMontage;
}; 