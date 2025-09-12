// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Int_InteractableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UInt_InteractableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ANDRESD3V_INTERACTIONSYSTEM_API IInt_InteractableInterface
{
	GENERATED_BODY()

public:
	/**
	 * Gets the display name for the interaction.
	 * @return The display name as FText.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="AndresD3v|Interaction")
	FText GetInteractionDisplayName() const;
	virtual FText GetInteractionDisplayName_Implementation() const;
	
	/**
	 * Called when the actor is selected by the interaction system.
	 * @param Instigator The actor that initiated the interaction.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="AndresD3v|Interaction")
	void OnInteractionSelected(AActor* Instigator);

	/**
	 * Called when the actor is deselected by the interaction system.
	 * @param Instigator The instigating actor.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="AndresD3v|Interaction")
	void OnInteractionDeselected(AActor* Instigator);
	
	/**
	 * Called when an interaction starts with this actor.
	 * @param Instigator The actor that initiated the interaction.
	 * @param OptionIndex The index of the interaction option chosen.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="AndresD3v|Interaction")
	void OnInteractionStarted(AActor* Instigator, int32 OptionIndex);
	
	/**
	 * Called when interaction with actor ends
	 * @param Instigator The actor that initiated the interaction.
	 * @param OptionIndex The index of the interaction option that was used.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="AndresD3v|Interaction")
	void OnInteractionEnded(AActor* Instigator, int32 OptionIndex);

	/**
	 * Called when an interaction option is selected
	 * @param Instigator The instigating actor.
	 * @param OptionIndex The index of the interaction option chosen.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="AndresD3v|Interaction")
	void OnInteractionOptionSelected(AActor* Instigator, int32 OptionIndex);
};
