// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "MechanicsInterface.generated.h"

class ACauldronAltar;
class ARitualPosition;
class ARitualAltar;
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

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Ritual Query")
	ARitualAltar* GetRitualAltarActor() const;

	//start ritual request
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Ritual Query")
	void SendStartRitualRequest(ACharacter* RequestingCharacter);


	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Ritual Query")
	void SendPlayerInput(ACharacter* InputCharacter, const FGameplayTag& InputTag);

	// /***
	//  * Ritual Altar interface functions
	//  */
	// UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Ritual Interaction")
	// void HandleStartRitualRequest(ACharacter* RequestingCharacter);
	//
	//
	// UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Ritual Interaction")
	// void HandlePlayerInput(ACharacter* InputCharacter, const FGameplayTag& InputTag);
	//
	// UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Ritual Interaction")
	// void HandleSetRitualPosition(ARitualPosition* InRitualPosition);
	//
	// UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Ritual Interaction")
	// ARitualPosition* GetRitualPosition() const;
	//
	// UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Ritual Interaction")
	// void GetRitualPositionTag(FGameplayTag& OutPositionTag) const;
	//
	//
	//
	//
	//
	// /**
	//  * Ritual Position interface functions
	//  */
	// UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Ritual Interaction")
	// bool IsRitualPositionOccupied() const;
	//
	// UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Ritual Interaction")
	// void SetRitualPositionOccupied(ACharacter* OccupyingCharacter);
	//
	//
	// /**
	//  * Cauldron Altar interface functions
	//  */
	//
	// UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Cauldron Interaction")
	// ACauldronAltar* GetCauldronAltar();
	//
	// UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Cauldron Interaction")
	// void HandleSetCauldronAltar(ACauldronAltar* InCauldronAltar);
	// UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Cauldron Interaction")
	// void HandleRemoveCauldronAltar();
	//
	// UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Cauldron Interaction")
	// void HandleStartBrewingPotion(ACharacter* InteractingCharacter);
	//
	// UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Cauldron Interaction")
	// void HandleStartCarryCauldron(ACharacter* InteractingCharacter);
	//
	// UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Cauldron Interaction")
	// void HandleStopBrewingPotion(ACharacter* InteractingCharacter);
	//
	// UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Cauldron Interaction")
	// void HandleStopCarryCauldron(ACharacter* InteractingCharacter);


	
	
	
};
