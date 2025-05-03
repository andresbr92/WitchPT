// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionOption.h"
#include "AbilitySystem/GameplayAbilities/WitchPTGameplayAbility.h"
#include "GameplayAbility_Interact.generated.h"

class UObject;
struct FFrame;
struct FGameplayAbilityActorInfo;
struct FGameplayEventData;

// Delegado para notificar cuando se completa una interacción
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractionComplete);

/**
 * 
 */
UCLASS(Abstract)
class WITCHPT_API UGameplayAbility_Interact : public UWitchPTGameplayAbility
{
	GENERATED_BODY()
	UGameplayAbility_Interact(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable)
	void UpdateInteractions(const TArray<FInteractionOption>& InteractiveOptions);
	
	// Método para interacción de pulsación rápida
	UFUNCTION(BlueprintCallable)
	void TriggerInteraction();
	
	// Método para interacción manteniendo pulsado
	UFUNCTION(BlueprintCallable)
	void TriggerHoldInteraction();
	
	// Método llamado cuando se presiona el botón de interacción
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void OnInteractionButtonPressed();
	
	// Método llamado cuando se suelta el botón de interacción
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void OnInteractionButtonReleased();
	
	// Método llamado cuando se cumple el tiempo de interacción mantenida
	UFUNCTION()
	void OnHoldInteractionTimeElapsed();

	// Delegado que se dispara cuando se completa una interacción
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnInteractionComplete OnInteractionComplete;

	// Delegado que se dispara cuando se completa una interacción mantenida
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnInteractionComplete OnHoldInteractionComplete;

protected:

	UPROPERTY(BlueprintReadWrite)
	TArray<FInteractionOption> CurrentOptions;

	UPROPERTY(EditDefaultsOnly)
	float InteractionScanRate = 0.1f;

	UPROPERTY(EditDefaultsOnly)
	float InteractionScanRange = 500;
	
	// Tiempo mínimo que debe mantenerse presionado el botón para considerar una interacción mantenida
	UPROPERTY(EditDefaultsOnly, Category="Interaction")
	float HoldInteractionTime = 1.0f;
	
	// Temporizador para detectar interacción mantenida
	FTimerHandle HoldInteractionTimerHandle;
	
	// Estado del botón de interacción (presionado o no)
	bool bIsInteractionButtonHeld = false;
	
	// Tiempo en que se inició la pulsación
	float InteractionStartTime = 0.0f;

	// UPROPERTY(EditDefaultsOnly)
	// TSoftClassPtr<UUserWidget> DefaultInteractionWidgetClass;

	
};
