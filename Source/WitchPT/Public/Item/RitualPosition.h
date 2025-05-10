// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/BaseInteractionPosition.h"
#include "GameplayTagContainer.h" // Added for FGameplayTag
#include "MechanicsInterface.h" // For GetRitualAltarActor & IsOccuppied if still needed
#include "PositionInterface.h"
#include "RitualPosition.generated.h"

// Forward Declarations
class ARitualAltar;
class UAbilitySystemComponent;
struct FGameplayAbilitySpec;
class AController;
class UGameplayAbility;
class UBoxComponent;
class ACharacter;

UCLASS()
class WITCHPT_API ARitualPosition : public ABaseInteractionPosition, public IPositionInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARitualPosition();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	//~ Begin AActor Interface
	virtual void BeginPlay() override;
	//~ End AActor Interface

	//~ Begin IInteraction Interface (from AItem)
	virtual void GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& OptionBuilder) override;
	//~ End IInteraction Interface
	
	// Still part of IMechanicsInterface if these are general queries
	virtual ARitualAltar* GetRitualAltarActor_Implementation() const override;
   

	void SetRitualAltar(ARitualAltar* Altar) { RitualAltar = Altar; }
	virtual bool IsOccupied_Implementation() override;

protected:
	// Reference to the parent altar
	UPROPERTY(EditInstanceOnly, Replicated)
	TObjectPtr<ARitualAltar> RitualAltar;

	// Interaction Logic Helper
	virtual void HandleInteraction(ACharacter* InteractingCharacter) override;

private:
	// Find the RitualAltar if not set
	
};
