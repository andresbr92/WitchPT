// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AT_WaitForPlacementLocation_SLT.generated.h"

// Delegado para enviar la información de la ubicación y la normal
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPlacementLocationFound, FVector, HitLocation, FVector, HitNormal);

/**
 * Task que realiza un line trace continuo para encontrar ubicaciones válidas para colocar el caldero.
 * Similar a UAT_WaitForInteractable_SLT pero devuelve información de ubicación y normal de la superficie.
 */
UCLASS()
class WITCHPT_API UAT_WaitForPlacementLocation_SLT : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	// Constructor
	UAT_WaitForPlacementLocation_SLT(const FObjectInitializer& ObjectInitializer);

	// Crea y configura la tarea
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAT_WaitForPlacementLocation_SLT* WaitForPlacementLocation(
		UGameplayAbility* OwningAbility,
		float TraceDistance = 500.0f,
		float TraceRadius = 10.0f,
		bool TraceComplex = false,
		bool bIgnoreBlockingHits = false,
		bool bShowDebug = false);

	// Delegado llamado cuando se encuentra una ubicación válida
	UPROPERTY(BlueprintAssignable)
	FPlacementLocationFound OnLocationFound;

	// Se llama cuando se activa la tarea
	virtual void Activate() override;

	// Se llama cuando se termina la tarea
	virtual void OnDestroy(bool bInOwnerFinished) override;

private:
	// Realiza el line trace para encontrar ubicaciones
	void PerformTrace();

	// Realiza un line trace cada tick para actualizar la ubicación
	void TickTask(float DeltaTime);

	// Temporizador para realizar el trace
	FTimerHandle TraceTimerHandle;

	// Distancia del trace
	float TraceDistance;

	// Radio del trace (para spheretrace)
	float TraceRadius;

	// Si se debe considerar geometría compleja
	bool bTraceComplex;

	// Si se deben ignorar hits bloqueantes
	bool bIgnoreBlockingHits;

	// Si se debe mostrar debug visual
	bool bShowDebug;

	// Última ubicación de hit encontrada
	FVector LastHitLocation;

	// Última normal encontrada
	FVector LastHitNormal;
}; 