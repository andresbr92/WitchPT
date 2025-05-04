// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/BaseInteractableAltar.h"
#include "GameplayTagContainer.h" // Include for FGameplayTag
#include "AbilitySystem/Interaction/IInteractableTarget.h"
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
class WITCHPT_API ACauldronAltar : public ABaseInteractableAltar, public IInteractableTarget
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ACauldronAltar();
    virtual void GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& OptionBuilder) override;
    UPROPERTY(EditAnywhere)
    FInteractionOption Option;
    
    virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
    
    UPROPERTY(ReplicatedUsing = OnRep_CauldronPhysicState, BlueprintReadWrite, VisibleAnywhere, Category = "Cauldron")
    TEnumAsByte<ECauldronPhysicState> CauldronPhysicState;

    // --- New functions for interaction ---
    
    /**
     * Function called when the player performs a quick interaction with the cauldron
     * This will position the player for potion making
     * @param InteractingCharacter The character that is interacting with the cauldron
     */
    UFUNCTION(BlueprintCallable, Category = "Cauldron|Interaction")
    void OnPressInteraction(ACharacter* InteractingCharacter);
    
    /**
     * Function called when the player performs a hold interaction with the cauldron
     * This will allow the player to carry the cauldron on their back
     * @param InteractingCharacter The character that is interacting with the cauldron
     */
    UFUNCTION(BlueprintCallable, Category = "Cauldron|Interaction")
    void OnHoldInteraction(ACharacter* InteractingCharacter);
    
    /**
     * Attaches the cauldron to the player's back
     * @param Character The character to attach the cauldron to
     */
    UFUNCTION(BlueprintCallable, Category = "Cauldron|Movement")
    void AttachToCharacter(ACharacter* Character);
    
    /**
     * Detaches the cauldron from the player and places it in the world
     * @param Character The character the cauldron is currently attached to
     */
    UFUNCTION(BlueprintCallable, Category = "Cauldron|Movement")
    void DetachFromCharacter(ACharacter* Character);
    
    /**
     * Positions the character at one of the cauldron positions for brewing
     * @param Character The character to position at the cauldron
     * @return Whether the character was successfully positioned
     */
    UFUNCTION(BlueprintCallable, Category = "Cauldron|Brewing")
    bool PositionCharacterForBrewing(ACharacter* Character);
    
    /**
     * Checks if the cauldron can be picked up
     * @return True if the cauldron can be picked up
     */
    UFUNCTION(BlueprintPure, Category = "Cauldron|State")
    bool CanBePickedUp() const;
    
    /**
     * Checks if the cauldron is currently being carried
     * @return True if the cauldron is being carried
     */
    UFUNCTION(BlueprintPure, Category = "Cauldron|State")
    bool IsBeingCarried() const;
    
    /**
     * Gets the character currently carrying the cauldron
     * @return The character carrying the cauldron, or nullptr if not being carried
     */
    UFUNCTION(BlueprintPure, Category = "Cauldron|State")
    ACharacter* GetCarryingCharacter() const;
    
    /**
     * Create brewing positions around the cauldron
     * These are scene components in the Blueprint that define where players can stand
     */
    UFUNCTION(BlueprintCallable, Category = "Cauldron|Brewing")
    void CreateBrewingPositions();
    
    /**
     * Removes all brewing positions created by the cauldron
     */
    UFUNCTION(BlueprintCallable, Category = "Cauldron|Brewing")
    void DestroyBrewingPositions();
    
    /**
     * Get an available brewing position for a character
     * @param Character The character looking for a brewing position
     * @return A valid brewing position, or nullptr if none available
     */
    UFUNCTION(BlueprintCallable, Category = "Cauldron|Brewing")
    ACauldronPosition* GetAvailableBrewingPosition(ACharacter* Character);

    UFUNCTION()
    void OnRep_CauldronPhysicState();
    
    // Transforms defining brewing positions around the cauldron
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cauldron|Brewing", meta = (MakeEditWidget = true))
    TArray<FTransform> BrewingPositionTransforms;
    
    // The class to spawn for brewing positions
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Cauldron|Brewing")
    TSubclassOf<ACauldronPosition> CauldronPositionClass;
    
private:
    // Character currently carrying the cauldron
    UPROPERTY(Replicated)
    TObjectPtr<ACharacter> CarryingCharacter;
    
    // Socket name for attaching the cauldron to the character
    UPROPERTY(EditDefaultsOnly, Category = "Cauldron|Movement")
    FName BackAttachSocketName = "BackpackSocket";
    
    // Offset for placing the cauldron when detached
    UPROPERTY(EditDefaultsOnly, Category = "Cauldron|Movement")
    FVector DetachmentOffset = FVector(100.0f, 0.0f, 0.0f);
    
    // Array of spawned brewing positions
    UPROPERTY(VisibleAnywhere)
    TArray<ACauldronPosition*> SpawnedBrewingPositions;
}; 