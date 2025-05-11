// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionOption.h"
#include "AbilitySystem/GameplayAbilities/WitchPTGameplayAbility.h"
#include "GameplayAbility_Interact.generated.h"

class UIndicatorDescriptor;
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
    
    // Método que verifica si el objeto bajo el trace sigue siendo el mismo
    UFUNCTION()
    void CheckInteractionValidity();
    
    // Método que cancela una interacción en curso
    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void CancelCurrentInteraction();
    
    // Método de seguridad que se activa si una interacción dura demasiado tiempo
    UFUNCTION()
    void OnMaxInteractionTimeElapsed();

	// Delegado que se dispara cuando se completa una interacción
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnInteractionComplete OnInteractionComplete;

	// Delegado que se dispara cuando se completa una interacción mantenida
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnInteractionComplete OnHoldInteractionComplete;
    
    // Delegado que se dispara cuando se cancela una interacción
    UPROPERTY(BlueprintAssignable, Category = "Interaction")
    FOnInteractionComplete OnInteractionCancelled;

protected:

	UPROPERTY(BlueprintReadWrite)
	TArray<FInteractionOption> CurrentOptions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Interaction", meta=(ClampMin="0.1"))
	float InteractionScanRate = 0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Interaction", meta=(ClampMin="1.0"))
	float InteractionScanRange = 500;
	
	// Tiempo mínimo que debe mantenerse presionado el botón para considerar una interacción mantenida
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Interaction", meta=(ClampMin="0.1", ToolTip="Tiempo en segundos que el jugador debe mantener presionado el botón para activar una interacción mantenida"))
	float HoldInteractionTime = 1.0f;
    
    // Frecuencia para verificar si el objeto interactuable sigue siendo válido durante una interacción mantenida
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Interaction", meta=(ClampMin="0.05", ToolTip="Tiempo en segundos entre comprobaciones de validez del objeto interactuable"))
    float ValidityCheckRate = 0.1f;
    
    // Tiempo máximo que puede durar cualquier interacción antes de ser cancelada automáticamente
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Interaction", meta=(ClampMin="1.0", ToolTip="Tiempo máximo en segundos que puede durar una interacción antes de ser cancelada automáticamente"))
    float MaxInteractionTime = 10.0f;
	
	// Temporizador para detectar interacción mantenida
	FTimerHandle HoldInteractionTimerHandle;
    
    // Temporizador para verificar la validez del objeto interactuable
    FTimerHandle ValidityCheckTimerHandle;
    
    // Temporizador de seguridad para forzar el reinicio después de un tiempo máximo
    FTimerHandle MaxInteractionTimeTimerHandle;
	
	// Estado del botón de interacción (presionado o no)
	bool bIsInteractionButtonHeld = false;
	
	// Tiempo en que se inició la pulsación
	float InteractionStartTime = 0.0f;
	
	// Indica si ya se disparó una interacción mantenida para la pulsación actual
	bool bHoldInteractionFired = false;
    
    // Guarda el objeto con el que se inició la interacción para compararlo
    TScriptInterface<IInteractableTarget> InitialInteractableTarget;

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UUserWidget> DefaultInteractionWidgetClass;

	UPROPERTY()
	TArray<TObjectPtr<UIndicatorDescriptor>> Indicators;

	
};
