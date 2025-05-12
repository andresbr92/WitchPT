// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Interaction/GameplayAbility_Interact.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/Interaction/AbilityTask_GrantNearbyInteraction.h"
#include "AbilitySystem/Interaction/InteractionStatics.h"
#include "NativeGameplayTags.h"
#include "AbilitySystem/WitchPTAbilitySystemComponent.h"
#include "AbilitySystem/Interaction/IInteractableTarget.h"
#include "Engine/World.h"
#include "Player/WitchPTPlayerController.h"

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Ability_Interaction_Activate, "Ability.Interaction.Activate");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Ability_Interaction_Hold_Activate, "Ability.Interaction.HoldActivate");

UGameplayAbility_Interact::UGameplayAbility_Interact(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ActivationPolicy = EPrototypeAbilityActivationPolicy::OnSpawn;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UGameplayAbility_Interact::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	UAbilitySystemComponent* AbilitySystem = GetAbilitySystemComponentFromActorInfo();
	UWitchPTAbilitySystemComponent* CustomAbilitySystemComponent = Cast<UWitchPTAbilitySystemComponent>(AbilitySystem);
	if (CustomAbilitySystemComponent && CustomAbilitySystemComponent->GetOwnerRole() == ROLE_Authority)
	{
		
		UAbilityTask_GrantNearbyInteraction* Task = UAbilityTask_GrantNearbyInteraction::GrantAbilitiesForNearbyInteractors(this, InteractionScanRange, InteractionScanRate);
		Task->ReadyForActivation();
		
	}
}

void UGameplayAbility_Interact::UpdateInteractions(const TArray<FInteractionOption>& InteractiveOptions)
{
	
	if (CurrentActorInfo && CurrentActorInfo->PlayerController.IsValid())
	{
		
		
		AWitchPTPlayerController* WitchPt = Cast<AWitchPTPlayerController>(CurrentActorInfo->PlayerController.Get());
		
		CurrentOptions = InteractiveOptions;
		
		for (const FInteractionOption& InteractionOption : InteractiveOptions)
		{
			AActor* InteractableTargetActor = UInteractionStatics::GetActorFromInteractableTarget(InteractionOption.InteractableTarget);
		
			if (InteractableTargetActor)
			{
				FString ActorName = InteractableTargetActor->GetName();
				WitchPt->HUDWidget->ShowPickupMessage(InteractionOption.Text.ToString());
			}
			
			
		}
		if (InteractiveOptions.Num() == 0)
		{
			WitchPt->HUDWidget->HidePickupMessage();
		}

	}
	
	CurrentOptions = InteractiveOptions;
}

void UGameplayAbility_Interact::TriggerInteraction()
{
	if (CurrentOptions.Num() == 0)
	{
		return;
	}

	UAbilitySystemComponent* AbilitySystem = GetAbilitySystemComponentFromActorInfo();
	if (AbilitySystem)
	{
		const FInteractionOption& InteractionOption = CurrentOptions[0];

		AActor* Instigator = GetAvatarActorFromActorInfo();
		AActor* InteractableTargetActor = UInteractionStatics::GetActorFromInteractableTarget(InteractionOption.InteractableTarget);

		// Allow the target to customize the event data we're about to pass in, in case the ability needs custom data
		// that only the actor knows.
		FGameplayEventData Payload;
		Payload.EventTag = TAG_Ability_Interaction_Activate;
		Payload.Instigator = Instigator;
		Payload.Target = InteractableTargetActor;

		// If needed we allow the interactable target to manipulate the event data so that for example, a button on the wall
		// may want to specify a door actor to execute the ability on, so it might choose to override Target to be the
		// door actor.
		InteractionOption.InteractableTarget->CustomizeInteractionEventData(TAG_Ability_Interaction_Activate, Payload);
		

		// Grab the target actor off the payload we're going to use it as the 'avatar' for the interaction, and the
		// source InteractableTarget actor as the owner actor.
		AActor* TargetActor = const_cast<AActor*>(ToRawPtr(Payload.Target));

		// The actor info needed for the interaction.
		FGameplayAbilityActorInfo ActorInfo;
		ActorInfo.InitFromActor(InteractableTargetActor, TargetActor, InteractionOption.TargetAbilitySystem);

		// Trigger the ability using event tag.
		
		const bool bSuccess = InteractionOption.TargetAbilitySystem->TriggerAbilityFromGameplayEvent(
			InteractionOption.TargetInteractionAbilityHandle,
			&ActorInfo,
			TAG_Ability_Interaction_Activate,
			&Payload,
			*InteractionOption.TargetAbilitySystem
		);
		
		// Notificar que se completó la interacción
		OnInteractionComplete.Broadcast();
	}
}

void UGameplayAbility_Interact::TriggerHoldInteraction()
{
	if (CurrentOptions.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("TriggerHoldInteraction: No hay opciones de interacción disponibles"));
		OnInteractionCancelled.Broadcast();
		return;
	}

	UAbilitySystemComponent* AbilitySystem = GetAbilitySystemComponentFromActorInfo();
	if (AbilitySystem)
	{
		const FInteractionOption& InteractionOption = CurrentOptions[0];

		// Verificar si la opción de interacción soporta interacción mantenida
		if (!InteractionOption.bSupportsHoldInteraction)
		{
			UE_LOG(LogTemp, Warning, TEXT("TriggerHoldInteraction: El objeto no soporta interacción mantenida, usando interacción normal"));
			// Si no soporta interacción mantenida, podemos usar la interacción normal en su lugar
			TriggerInteraction();
			return;
		}

		UE_LOG(LogTemp, Warning, TEXT("TriggerHoldInteraction: Activando interacción mantenida en objeto que la soporta"));
		
		AActor* Instigator = GetAvatarActorFromActorInfo();
		AActor* InteractableTargetActor = UInteractionStatics::GetActorFromInteractableTarget(InteractionOption.InteractableTarget);

		// Configurar los datos del evento para la interacción mantenida
		FGameplayEventData Payload;
		Payload.EventTag = TAG_Ability_Interaction_Hold_Activate;
		Payload.Instigator = Instigator;
		Payload.Target = InteractableTargetActor;

		// Permitir que el objetivo personalice los datos del evento
		InteractionOption.InteractableTarget->CustomizeInteractionEventData(TAG_Ability_Interaction_Hold_Activate, Payload);

		// Obtener el actor objetivo
		AActor* TargetActor = const_cast<AActor*>(ToRawPtr(Payload.Target));

		// La información del actor necesaria para la interacción
		FGameplayAbilityActorInfo ActorInfo;
		ActorInfo.InitFromActor(InteractableTargetActor, TargetActor, InteractionOption.TargetAbilitySystem);

		// Usar el handle de habilidad específico para interacción mantenida, si está disponible
		FGameplayAbilitySpecHandle AbilityHandle = InteractionOption.TargetHoldInteractionAbilityHandle;
		
		// Si no hay un handle específico para interacción mantenida, usar el handle normal
		if (!AbilityHandle.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("TriggerHoldInteraction: No hay handle específico para interacción mantenida, usando el handle normal"));
			AbilityHandle = InteractionOption.TargetInteractionAbilityHandle;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("TriggerHoldInteraction: Usando handle específico para interacción mantenida"));
		}

		// Activar la habilidad usando la etiqueta de evento de interacción mantenida
		// UE_LOG(LogTemp, Warning, TEXT("TriggerHoldInteraction: Intentando activar habilidad con tag %s"), *TAG_Ability_Interaction_Hold_Activate.ToString());
		const bool bSuccess = InteractionOption.TargetAbilitySystem->TriggerAbilityFromGameplayEvent(
			AbilityHandle,
			&ActorInfo,
			TAG_Ability_Interaction_Hold_Activate,
			&Payload,
			*InteractionOption.TargetAbilitySystem
		);
		
		UE_LOG(LogTemp, Warning, TEXT("TriggerHoldInteraction: Resultado de activación: %s"), bSuccess ? TEXT("Éxito") : TEXT("Fallido"));
		
		// Notificar que se completó la interacción mantenida
		OnHoldInteractionComplete.Broadcast();
	}
}

void UGameplayAbility_Interact::OnInteractionButtonPressed()
{
	if (CurrentOptions.Num() == 0)
	{
		// UE_LOG(LogTemp, Warning, TEXT("GameplayAbility_Interact: No hay opciones de interacción disponibles"));
		OnInteractionCancelled.Broadcast();
		return;
	}
	
	// UE_LOG(LogTemp, Warning, TEXT("GameplayAbility_Interact: Botón de interacción presionado"));
	bIsInteractionButtonHeld = true;
	bHoldInteractionFired = false; // Reiniciamos la bandera al comenzar una nueva interacción
	InteractionStartTime = GetWorld()->GetTimeSeconds();
	
	// Guardar el objetivo interactuable actual para validaciones posteriores
	if (CurrentOptions.Num() > 0)
	{
		InitialInteractableTarget = CurrentOptions[0].InteractableTarget;
		// UE_LOG(LogTemp, Warning, TEXT("GameplayAbility_Interact: Guardando objetivo inicial de interacción"));
	}
	
	// Configurar un temporizador para detectar si el botón se mantiene presionado durante el tiempo requerido
	GetWorld()->GetTimerManager().SetTimer(
		HoldInteractionTimerHandle,
		this,
		&UGameplayAbility_Interact::OnHoldInteractionTimeElapsed,
		HoldInteractionTime,
		false
	);
	
	// Iniciar comprobación periódica de validez
	GetWorld()->GetTimerManager().SetTimer(
		ValidityCheckTimerHandle,
		this,
		&UGameplayAbility_Interact::CheckInteractionValidity,
		ValidityCheckRate,
		true // Repetitivo
	);
	
	// Configurar el temporizador de seguridad para forzar un reinicio después de un tiempo máximo
	GetWorld()->GetTimerManager().SetTimer(
		MaxInteractionTimeTimerHandle,
		this,
		&UGameplayAbility_Interact::OnMaxInteractionTimeElapsed,
		MaxInteractionTime,
		false // No repetitivo
	);
	
	// UE_LOG(LogTemp, Warning, TEXT("GameplayAbility_Interact: Temporizadores configurados. Hold: %f segundos, Max: %f segundos"), 
	// 	HoldInteractionTime, MaxInteractionTime);
}

void UGameplayAbility_Interact::OnInteractionButtonReleased()
{
	// UE_LOG(LogTemp, Warning, TEXT("GameplayAbility_Interact: Botón de interacción liberado"));
	
	if (!bIsInteractionButtonHeld)
	{
		// UE_LOG(LogTemp, Warning, TEXT("GameplayAbility_Interact: El botón no estaba siendo mantenido, ignorando liberación"));
		return;
	}
	
	bIsInteractionButtonHeld = false;
	
	// Cancelar los temporizadores
	if (GetWorld())
	{
		// Temporizador de validez
		if (GetWorld()->GetTimerManager().IsTimerActive(ValidityCheckTimerHandle))
		{
			GetWorld()->GetTimerManager().ClearTimer(ValidityCheckTimerHandle);
		}
		
		// Temporizador de seguridad
		if (GetWorld()->GetTimerManager().IsTimerActive(MaxInteractionTimeTimerHandle))
		{
			GetWorld()->GetTimerManager().ClearTimer(MaxInteractionTimeTimerHandle);
		}
		
		// Temporizador de interacción mantenida
		if (GetWorld()->GetTimerManager().IsTimerActive(HoldInteractionTimerHandle))
		{
			// UE_LOG(LogTemp, Warning, TEXT("GameplayAbility_Interact: Temporizador de hold cancelado"));
			GetWorld()->GetTimerManager().ClearTimer(HoldInteractionTimerHandle);
			
			// Verificamos si ya se lanzó una interacción mantenida
			if (bHoldInteractionFired)
			{
				// UE_LOG(LogTemp, Warning, TEXT("GameplayAbility_Interact: Ya se disparó una interacción mantenida, ignorando interacción rápida"));
				return; // Si ya se disparó una interacción mantenida, no lanzamos la rápida
			}
			
			// Si el botón se soltó antes del tiempo de interacción mantenida, considerarlo como una pulsación rápida
			float CurrentTime = GetWorld()->GetTimeSeconds();
			float HoldTime = CurrentTime - InteractionStartTime;
			
			// UE_LOG(LogTemp, Warning, TEXT("GameplayAbility_Interact: Tiempo mantenido: %f / Requerido: %f"), HoldTime, HoldInteractionTime);
			
			if (HoldTime < HoldInteractionTime)
			{
				// UE_LOG(LogTemp, Warning, TEXT("GameplayAbility_Interact: Activando interacción rápida"));
				// Llamar a la interacción de pulsación rápida
				TriggerInteraction();
				
				// Llamar a InteractPressScan después de la interacción para restaurar el estado
				// y prepararse para la siguiente interacción
				// Nota: Esto debe implementarse en el blueprint conectando la salida de este nodo
				// a la entrada del nodo InteractPressScan
			}
		}
		else
		{
			// UE_LOG(LogTemp, Warning, TEXT("GameplayAbility_Interact: No había temporizador activo al liberar el botón"));
		}
	}
	
	// Limpiar la referencia al objetivo inicial
	InitialInteractableTarget = nullptr;
}

void UGameplayAbility_Interact::OnHoldInteractionTimeElapsed()
{
	// UE_LOG(LogTemp, Warning, TEXT("GameplayAbility_Interact: Tiempo de interacción mantenida alcanzado"));
	
	if (bIsInteractionButtonHeld)
	{
		// UE_LOG(LogTemp, Warning, TEXT("GameplayAbility_Interact: Activando interacción mantenida"));
		// Marcamos que ya se disparó la interacción mantenida
		bHoldInteractionFired = true;
		
		// El botón se ha mantenido presionado durante el tiempo requerido
		// Llamar a la interacción mantenida
		TriggerHoldInteraction();
		
		// Nota: Después de llamar a TriggerHoldInteraction, 
		// es necesario llamar a InteractPressScan en el blueprint
		// para restaurar el estado y prepararse para la siguiente interacción
		// Esto se logra escuchando al delegado OnHoldInteractionComplete
	}
	else
	{
		// UE_LOG(LogTemp, Warning, TEXT("GameplayAbility_Interact: El botón ya no está presionado, no se activa interacción mantenida"));
	}
}

void UGameplayAbility_Interact::CheckInteractionValidity()
{
	// Si no estamos en un estado de interacción, no necesitamos verificar
	if (!bIsInteractionButtonHeld || !InitialInteractableTarget.GetInterface())
	{
		return;
	}
	
	// Log adicional para verificar estado actual
	UE_LOG(LogTemp, Verbose, TEXT("CheckInteractionValidity: Verificando validez de interacción. Opciones actuales: %d"), CurrentOptions.Num());
	
	// Verificar si aún hay opciones de interacción disponibles
	bool bHasValidInteraction = false;
	
	if (CurrentOptions.Num() > 0)
	{
		// Buscar el objetivo inicial en cualquier posición del array
		for (const FInteractionOption& Option : CurrentOptions)
		{
			if (Option.InteractableTarget == InitialInteractableTarget)
			{
				bHasValidInteraction = true;
				break;
			}
		}
		
		// Log detallado sobre la validación
		if (bHasValidInteraction)
		{
			UE_LOG(LogTemp, Verbose, TEXT("CheckInteractionValidity: Objetivo de interacción aún es válido"));
		}
		else
		{
			// Intentar identificar qué cambió
			AActor* InitialActor = UInteractionStatics::GetActorFromInteractableTarget(InitialInteractableTarget);
			AActor* CurrentActor = CurrentOptions.Num() > 0 ? 
				UInteractionStatics::GetActorFromInteractableTarget(CurrentOptions[0].InteractableTarget) : nullptr;
			
			UE_LOG(LogTemp, Warning, TEXT("CheckInteractionValidity: Objetivo de interacción inválido. Inicial: %s, Actual: %s"), 
				InitialActor ? *InitialActor->GetName() : TEXT("Ninguno"),
				CurrentActor ? *CurrentActor->GetName() : TEXT("Ninguno"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CheckInteractionValidity: No hay opciones de interacción disponibles"));
	}
	
	// Si no hay una interacción válida, cancelar la interacción actual
	if (!bHasValidInteraction)
	{
		UE_LOG(LogTemp, Warning, TEXT("CheckInteractionValidity: El objetivo de interacción ha cambiado o no es válido, cancelando interacción"));
		CancelCurrentInteraction();
	}
}

void UGameplayAbility_Interact::CancelCurrentInteraction()
{
	// Si no estamos en un estado de interacción, no hay nada que cancelar
	if (!bIsInteractionButtonHeld)
	{
		UE_LOG(LogTemp, Verbose, TEXT("CancelCurrentInteraction: No hay interacción activa para cancelar"));
		return;
	}
	
	// Obtener información del objeto inicial para el log
	FString InitialObjectName = TEXT("Desconocido");
	if (InitialInteractableTarget.GetInterface())
	{
		AActor* InitialActor = UInteractionStatics::GetActorFromInteractableTarget(InitialInteractableTarget);
		if (InitialActor)
		{
			InitialObjectName = InitialActor->GetName();
		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("GameplayAbility_Interact: Cancelando interacción actual con %s"), *InitialObjectName);
	
	// Detener temporizadores
	if (GetWorld())
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(ValidityCheckTimerHandle))
		{
			GetWorld()->GetTimerManager().ClearTimer(ValidityCheckTimerHandle);
			UE_LOG(LogTemp, Verbose, TEXT("CancelCurrentInteraction: Temporizador de validez detenido"));
		}
		
		if (GetWorld()->GetTimerManager().IsTimerActive(HoldInteractionTimerHandle))
		{
			GetWorld()->GetTimerManager().ClearTimer(HoldInteractionTimerHandle);
			UE_LOG(LogTemp, Verbose, TEXT("CancelCurrentInteraction: Temporizador de interacción mantenida detenido"));
		}
		
		if (GetWorld()->GetTimerManager().IsTimerActive(MaxInteractionTimeTimerHandle))
		{
			GetWorld()->GetTimerManager().ClearTimer(MaxInteractionTimeTimerHandle);
			UE_LOG(LogTemp, Verbose, TEXT("CancelCurrentInteraction: Temporizador de seguridad detenido"));
		}
	}
	
	// Restablecer variables de estado
	bIsInteractionButtonHeld = false;
	bHoldInteractionFired = false;
	
	// Hacer una copia de la referencia antes de limpiarla para poder notificar
	TScriptInterface<IInteractableTarget> CanceledTarget = InitialInteractableTarget;
	InitialInteractableTarget = nullptr;
	
	// Log para confirmar que el estado ha sido restablecido
	UE_LOG(LogTemp, Log, TEXT("GameplayAbility_Interact: Estado de interacción restablecido completamente"));
	
	// Notificar la cancelación
	OnInteractionCancelled.Broadcast();
}

void UGameplayAbility_Interact::OnMaxInteractionTimeElapsed()
{
	// Si todavía estamos en un estado de interacción, forzar una cancelación
	if (bIsInteractionButtonHeld)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameplayAbility_Interact: ¡Tiempo máximo de interacción alcanzado! Forzando cancelación después de %f segundos"), MaxInteractionTime);
		CancelCurrentInteraction();
		
		// Forzar un reinicio del escaneo de interacciones
		OnInteractionCancelled.Broadcast();
	}
}
