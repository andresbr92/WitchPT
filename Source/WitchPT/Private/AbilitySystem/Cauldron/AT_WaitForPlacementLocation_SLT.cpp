// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Cauldron/AT_WaitForPlacementLocation_SLT.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"

UAT_WaitForPlacementLocation_SLT::UAT_WaitForPlacementLocation_SLT(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bTickingTask = true;
	TraceDistance = 500.0f;
	TraceRadius = 10.0f;
	bTraceComplex = false;
	bIgnoreBlockingHits = false;
	bShowDebug = false;
	LastHitLocation = FVector::ZeroVector;
	LastHitNormal = FVector::UpVector;
}

UAT_WaitForPlacementLocation_SLT* UAT_WaitForPlacementLocation_SLT::WaitForPlacementLocation(
	UGameplayAbility* OwningAbility,
	float TraceDistance,
	float TraceRadius,
	bool TraceComplex,
	bool bIgnoreBlockingHits,
	bool bShowDebug)
{
	UAT_WaitForPlacementLocation_SLT* MyObj = NewAbilityTask<UAT_WaitForPlacementLocation_SLT>(OwningAbility);
	
	MyObj->TraceDistance = TraceDistance;
	MyObj->TraceRadius = TraceRadius;
	MyObj->bTraceComplex = TraceComplex;
	MyObj->bIgnoreBlockingHits = bIgnoreBlockingHits;
	MyObj->bShowDebug = bShowDebug;
	
	return MyObj;
}

void UAT_WaitForPlacementLocation_SLT::Activate()
{
	Super::Activate();
	
	// Configurar para ejecutar el trace en cada tick
	SetWaitingOnAvatar();
	
	// Realizar un primer trace inmediatamente
	PerformTrace();
}

void UAT_WaitForPlacementLocation_SLT::OnDestroy(bool bInOwnerFinished)
{
	// Limpiar el temporizador si está activo
	UWorld* World = GetWorld();
	if (World)
	{
		World->GetTimerManager().ClearTimer(TraceTimerHandle);
	}
	
	Super::OnDestroy(bInOwnerFinished);
}

void UAT_WaitForPlacementLocation_SLT::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);
	
	// Realizar el trace en cada tick
	PerformTrace();
}

void UAT_WaitForPlacementLocation_SLT::PerformTrace()
{
	if (!Ability || !Ability->GetCurrentActorInfo())
	{
		return;
	}
	
	// Obtener el controlador del jugador
	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	if (!PC)
	{
		return;
	}
	
	// Obtener el personaje del jugador (necesario para obtener la cámara)
	ACharacter* Character = Cast<ACharacter>(Ability->GetCurrentActorInfo()->AvatarActor.Get());
	if (!Character)
	{
		return;
	}
	
	// Definir los puntos de inicio y fin del trace
	FVector TraceStart;
	FVector TraceDirection;
	
	// Buscar un componente de cámara en el personaje
	UCameraComponent* CameraComponent = Character->FindComponentByClass<UCameraComponent>();
	if (CameraComponent)
	{
		// Usar la posición y dirección de la cámara para el trace
		TraceStart = CameraComponent->GetComponentLocation();
		TraceDirection = CameraComponent->GetForwardVector();
	}
	else
	{
		// Si no hay cámara, usar la rotación del controlador
		FRotator ControllerRotation = PC->GetControlRotation();
		PC->GetPlayerViewPoint(TraceStart, ControllerRotation);
		TraceDirection = ControllerRotation.Vector();
	}
	
	FVector TraceEnd = TraceStart + TraceDirection * TraceDistance;
	
	// Configurar parámetros del trace
	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(WaitForPlacementLocation), bTraceComplex);
	QueryParams.AddIgnoredActor(Character);
	
	// Configurar qué tipos de objetos detectar
	ECollisionChannel TraceChannel = ECC_Visibility;
	
	// Realizar el trace
	FHitResult HitResult;
	bool bHit = false;
	
	// Realizar un sphere trace o line trace según el radio
	if (TraceRadius > 0.0f)
	{
		bHit = GetWorld()->SweepSingleByChannel(
			HitResult,
			TraceStart,
			TraceEnd,
			FQuat::Identity,
			TraceChannel,
			FCollisionShape::MakeSphere(TraceRadius),
			QueryParams
		);
	}
	else
	{
		bHit = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			TraceStart,
			TraceEnd,
			TraceChannel,
			QueryParams
		);
	}
	
	// Si encontramos un hit y es diferente al último que reportamos
	if (bHit && (HitResult.ImpactPoint != LastHitLocation || HitResult.ImpactNormal != LastHitNormal))
	{
		// Almacenar los nuevos valores
		LastHitLocation = HitResult.ImpactPoint;
		LastHitNormal = HitResult.ImpactNormal;
		
		// Disparar el delegado con la información del hit
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnLocationFound.Broadcast(LastHitLocation, LastHitNormal);
		}
	}
	
	// Mostrar debug si está habilitado
	if (bShowDebug)
	{
		DrawDebugLine(GetWorld(), TraceStart, bHit ? HitResult.ImpactPoint : TraceEnd, FColor::Green, false, 0.0f, 0, 2.0f);
		
		if (bHit)
		{
			// Dibujar una esfera en el punto de impacto
			DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 12, FColor::Red, false, 0.0f);
			
			// Dibujar la normal
			DrawDebugLine(GetWorld(), HitResult.ImpactPoint, HitResult.ImpactPoint + HitResult.ImpactNormal * 50.0f, FColor::Blue, false, 0.0f, 0, 2.0f);
		}
	}
} 