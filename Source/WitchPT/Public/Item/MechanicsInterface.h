// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "MechanicsInterface.generated.h"

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
	//RITUAL IMPLEMENTATION
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mechanics|Ritual")
	void RequestStartSequence(int32 SequenceIndex);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mechanics|Ritual")
	void RequestMoveToRitual(ACauldronAltar* TargetAltar);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mechanics|Ritual")
	void RequestRitualInput(ARitualAltar* TargetAltar, const FGameplayTag& InputTag);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mechanics|Ritual")
	void RequestStartRitual(ARitualAltar* TargetAltar);

	
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

	
	
	
	
	
	
	// LEGACY

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Ritual Query")
	ARitualAltar* GetRitualAltarActor() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Ritual Query")
	void SendStartRitualRequest(ACharacter* RequestingCharacter);


	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Ritual Query")
	void SendPlayerInput(ACharacter* InputCharacter, const FGameplayTag& InputTag);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Ritual Query")
	void SendPlayerOccupiedPosition(ACharacter* InputCharacter);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Ritual Queyr")
	bool IsOccuppied() const;


	// CAULDRON IMPLEMENTATION
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Cauldron Query")
	void SendStartBrewingPotionRequest(ACharacter* RequestingCharacter);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Cauldron Query")
	void SendStartCarryCauldronRequest(ACharacter* RequestingCharacter);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Cauldron Query")
	void SendStartPlacementPreview(ACharacter* RequestingCharacter);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Cauldron Query")
	void SendUpdatePlacementPreview(const FVector& HitLocation, const FVector& HitNormal);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Cauldron Query")
	void SendCancelPlacementPreview();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Cauldron Query")
	void SendFinalizePlacement();


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
