// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/BaseInteractionPosition.h"
#include "GameplayTagContainer.h" // Added for FGameplayTag
#include "RitualInterface.h"
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
class WITCHPT_API ARitualPosition : public ABaseInteractionPosition
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARitualPosition();

	//~ Begin AActor Interface
	virtual void BeginPlay() override;
	//~ End AActor Interface

	//~ Begin IInteraction Interface (from AItem)
	virtual void GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& OptionBuilder) override;
	//~ End IInteraction Interface

	//~ Getters
	UFUNCTION(BlueprintPure, Category = "Ritual")
	ARitualAltar* GetRitualAltar() const { return RitualAltar; }
	//~ End Getters

	//~ Setters
	UFUNCTION(BlueprintCallable)
	void SetRitualAltar(ARitualAltar* Altar) { RitualAltar = Altar; }
	//~ End Setters

protected:
	// Reference to the parent altar
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ritual", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ARitualAltar> RitualAltar;

	// Interaction Logic Helper
	virtual void HandleInteraction(ACharacter* InteractingCharacter) override;

private:
	// Find the RitualAltar if not set
	void FindRitualAltar();
};
