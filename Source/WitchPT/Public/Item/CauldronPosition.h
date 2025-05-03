// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/BaseInteractionPosition.h"
#include "GameplayTagContainer.h" // Added for FGameplayTag
#include "CauldronPosition.generated.h"

// Forward Declarations
class ACauldronAltar;
class UAbilitySystemComponent;
struct FGameplayAbilitySpec;
class AController;
class UGameplayAbility;
class UBoxComponent;
class ACharacter;

/**
 * Position for interacting with a Cauldron altar
 */
UCLASS()
class WITCHPT_API ACauldronPosition : public ABaseInteractionPosition
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ACauldronPosition();

    //~ Begin AActor Interface
    virtual void BeginPlay() override;
    //~ End AActor Interface

    //~ Begin IInteraction Interface (from AItem)
    virtual void GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& OptionBuilder) override;
    //~ End IInteraction Interface

    //~ Getters
    UFUNCTION(BlueprintPure, Category = "Cauldron")
    ACauldronAltar* GetCauldronAltar() const { return CauldronAltar; }
    //~ End Getters

    //~ Setters
    UFUNCTION(BlueprintCallable)
    void SetCauldronAltar(ACauldronAltar* Altar) { CauldronAltar = Altar; }
    //~ End Setters

protected:
    // Reference to the parent altar
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Cauldron", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<ACauldronAltar> CauldronAltar;

    // Interaction Logic Helper
    virtual void HandleInteraction(ACharacter* InteractingCharacter) override;

private:
    // Find the CauldronAltar if not set
    void FindCauldronAltar();
}; 