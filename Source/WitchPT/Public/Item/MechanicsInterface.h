// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "MechanicsInterface.generated.h"

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
	//RITUAL IMPLEMENTATION
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mechanics|Ritual")
	void RequestStartSequence(int32 SequenceIndex);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mechanics|Ritual")
	void RequestMoveToRitual(ACauldronAltar* TargetAltar);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mechanics|Ritual")
	void RequestRitualInput(ARitualAltar* TargetAltar, const FGameplayTag& InputTag);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mechanics|Ritual")
	void RequestStartRitual(ARitualAltar* TargetAltar);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mechanics|Ritual")
	void RequestOccupyPosition(ARitualAltar* TargetAltar, ABaseInteractionPosition* Position);

	
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

	
	// General Queries that might be implemented by positions or other interactables
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Mechanics|Query")
	ARitualAltar* GetRitualAltarActor() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Mechanics|Query")
	bool IsOccuppied() const;


	// CAULDRON LEGACY - These were for CauldronAltar to implement, now handled by WitchPTMechanicComponent
	// UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Cauldron Query")
	// void SendStartBrewingPotionRequest(ACharacter* RequestingCharacter);
	// UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Cauldron Query")
	// void SendStartCarryCauldronRequest(ACharacter* RequestingCharacter);
	// UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Cauldron Query")
	// void SendStartPlacementPreview(ACharacter* RequestingCharacter);
	// UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Cauldron Query")
	// void SendUpdatePlacementPreview(const FVector& HitLocation, const FVector& HitNormal);
	// UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Cauldron Query")
	// void SendCancelPlacementPreview();
	// UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Cauldron Query")
	// void SendFinalizePlacement();
};
