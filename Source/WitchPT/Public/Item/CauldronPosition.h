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
    //
    // /**
    //  * Posiciona un personaje en esta ubicación
    //  * @param Character El personaje a posicionar
    //  * @return True si el personaje fue posicionado correctamente
    //  */
    // UFUNCTION(BlueprintCallable, Category = "Cauldron|Interaction")
    // bool PositionCharacter(ACharacter* Character);
    //
    // /**
    //  * Comprueba si esta posición está ocupada
    //  * @return True si la posición está ocupada por un personaje
    //  */
    // UFUNCTION(BlueprintPure, Category = "Cauldron|State")
    // bool IsOccupied() const;
    //
    // /**
    //  * Establece el estado de ocupación de esta posición
    //  * @param bInOccupied Si la posición está ocupada
    //  * @param InOccupyingCharacter El personaje que ocupa la posición
    //  */
    // UFUNCTION(BlueprintCallable, Category = "Cauldron|State")
    // void SetOccupied(bool bInOccupied, ACharacter* InOccupyingCharacter = nullptr);
    //
    // /**
    //  * Obtiene el personaje que ocupa esta posición
    //  * @return El personaje que ocupa la posición, o nullptr si no está ocupada
    //  */
    // UFUNCTION(BlueprintPure, Category = "Cauldron|State")
    // ACharacter* GetOccupyingCharacter() const { return OccupyingCharacter; }

protected:
    // Reference to the parent altar
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Cauldron", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<ACauldronAltar> CauldronAltar;

    // Interaction Logic Helper
    virtual void HandleInteraction(ACharacter* InteractingCharacter) override;

private:
    // Find the CauldronAltar if not set
    void FindCauldronAltar();
    //
    // // Character currently occupying this position
    // UPROPERTY()
    // TObjectPtr<ACharacter> OccupyingCharacter;
    //
    // // Whether this position is currently occupied
    // UPROPERTY()
    // bool bIsOccupied;
}; 