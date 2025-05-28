// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "MechanicsInterface.generated.h"

class UWitchPTInventoryItemInstance;
class ABaseInteractionPosition;
class ACauldronAltar;
class ARitualPosition;
class ARitualAltar;
class ACharacter;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMechanicsInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class WITCHPT_API IMechanicsInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
	
public:
	// ------------------------------------ RITUAL IMPLEMENTATION ---------------------------------
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mechanics|Ritual")
	void RequestStartRitual(ARitualAltar* TargetAltar);
	

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mechanics|Ritual")
	void RequestRitualInput(ARitualAltar* TargetAltar, const FGameplayTag& InputTag);


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mechanics|Ritual")
	void RequestOccupyPosition(ARitualAltar* TargetAltar, ABaseInteractionPosition* Position);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mechanics|Ritual")
	void RequestUnOccupyPositionInRitual(ARitualAltar* TargetAltar, ABaseInteractionPosition* Position);
	
	// ----------------------------------- CAULDRON IMPLEMENTATION ---------------------------------
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mechanics|Cauldron")
	void RequestStartBrewingPotion(ACauldronAltar* TargetAltar);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mechanics|Cauldron")
	void RequestStartCarryCauldron(ACauldronAltar* TargetAltar);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mechanics|Cauldron")
	void RequestStartPlacementPreview(ACauldronAltar* TargetAltar);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mechanics|Cauldron")
	void RequestUpdatePlacementPreview(ACauldronAltar* TargetAltar, const FVector& HitLocation, const FVector& HitNormal);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mechanics|Cauldron")
	void RequestCancelPlacementPreview(ACauldronAltar* TargetAltar);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mechanics|Cauldron")
	void RequestFinalizePlacement(ACauldronAltar* TargetAltar);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mechanics|Cauldron")
	void RequestUnOccupyPositionInCauldron(ACauldronAltar* TargetCauldron);

	// ----------------------------------- CAULDRON INGREDIENT IMPLEMENTATION ---------------------------------
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mechanics|Cauldron")
	void RequestPlaceBaseIngredientInCauldron(ACauldronAltar* TargetAltar, UWitchPTInventoryItemInstance* IngredientInstance);

	
	// General Queries that might be implemented by positions or other interactables
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Mechanics|Query")
	ARitualAltar* GetRitualAltarActor() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Mechanics|Query")
	bool IsOccuppied() const;
	
};
