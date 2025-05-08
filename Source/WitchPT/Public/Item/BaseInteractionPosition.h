// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Item.h"
#include "AbilitySystem/Interaction/InteractionOption.h"
#include "GameplayTagContainer.h" // Added for FGameplayTag
#include "MechanicsInterface.h"
#include "BaseInteractionPosition.generated.h"

class UAbilitySystemComponent;
struct FGameplayAbilitySpec;
class AController;
class UGameplayAbility;
class UBoxComponent;
class ACharacter;
class ABaseInteractableAltar;

/**
 * Base class for interaction positions (rituals, cauldrons, etc)
 */
UCLASS(Abstract)
class WITCHPT_API ABaseInteractionPosition : public AItem, public IMechanicsInterface
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ABaseInteractionPosition();

    //~ Begin AActor Interface
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    virtual void BeginPlay() override;
    //~ End AActor Interface

    //~ Begin IInteraction Interface (from AItem presumably)
    virtual void GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& OptionBuilder) override;
    //~ End IInteraction Interface

    //~ Getters

    UFUNCTION(BlueprintPure, Category = "Interaction")
    ACharacter* GetOccupyingCharacter() const { return OccupyingCharacter; }

    UFUNCTION(BlueprintPure, Category = "Interaction")
    FGameplayTag GetPositionTag() const { return PositionTag; }
    //~ End Getters
    virtual bool IsOccuppied_Implementation() const override;

    //~ Setters
    UFUNCTION()
    void SetOccupied(ACharacter* Character);

    UFUNCTION()
    void SetUnoccupied();

    UFUNCTION()
    void SetPositionTag(const FGameplayTag& NewTag);

protected:
    // Unique tag identifying this position (e.g., Ritual.Position.1). Set per instance.
    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "Interaction")
    FGameplayTag PositionTag;

    // Is this position currently occupied by a player? Replicated.
    UPROPERTY(ReplicatedUsing = OnRep_IsOccupied, BlueprintReadOnly, Category = "Interaction|State")
    bool bIsOccupied = false;

    // Which character is currently occupying this position? Replicated.
    UPROPERTY(ReplicatedUsing = OnRep_OccupyingCharacter, VisibleAnywhere, BlueprintReadOnly, Category = "Interaction|State")
    TObjectPtr<ACharacter> OccupyingCharacter = nullptr;

    // Variables potentially used by the base AItem interaction system
    UPROPERTY()
    TWeakObjectPtr<UAbilitySystemComponent> InteractingASC;

    UPROPERTY()
    TWeakObjectPtr<AController> InteractingController;

    //~ Replication Notifies
    UFUNCTION()
    void OnRep_IsOccupied();

    UFUNCTION()
    void OnRep_OccupyingCharacter();
    //~ End Replication Notifies

    // Interaction Logic Helper (Called from GatherInteractionOptions or a specific interaction GA)
    virtual void HandleInteraction(ACharacter* InteractingCharacter);

private:
    void RemoveCharacterFromPosition();
}; 