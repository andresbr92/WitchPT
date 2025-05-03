// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Interaction/GameplayAbility_Interact.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/Interaction/AbilityTask_GrantNearbyInteraction.h"
#include "AbilitySystem/Interaction/InteractionStatics.h"
#include "NativeGameplayTags.h"
#include "AbilitySystem/WitchPTAbilitySystemComponent.h"
#include "AbilitySystem/Interaction/IInteractableTarget.h"
#include "Engine/World.h"

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
	// print options
	for (const FInteractionOption& InteractionOption : InteractiveOptions)
	{
		// print something
		// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Interaction Option"));
	}

	// Verifica si la habilidad tiene información de contexto válida
	if (CurrentActorInfo && CurrentActorInfo->PlayerController.IsValid())
	{
		// Obtén el PlayerController directamente del contexto de la habilidad
		APlayerController* PC = CurrentActorInfo->PlayerController.Get();

		// Limpia la lista de opciones actuales
		CurrentOptions = InteractiveOptions;
		// show length of array
		

		// Itera sobre las opciones de interacción detectadas
		for (const FInteractionOption& InteractionOption : InteractiveOptions)
		{
			// Obtén el actor asociado a esta opción de interacción
			AActor* InteractableTargetActor = UInteractionStatics::GetActorFromInteractableTarget(InteractionOption.InteractableTarget);
		
			if (InteractableTargetActor)
			{
				// Muestra un mensaje con el nombre del actor interactuable
				FString ActorName = InteractableTargetActor->GetName();
				// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Objeto interactuable detectado: %s"), *ActorName));
			}
			else
			{
				// Mensaje de error si no se puede obtener el actor
				// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("No se pudo obtener el actor interactuable."));
			}
		}

		// Mensaje para indicar que las opciones de interacción se actualizaron
		// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Opciones de interacción actualizadas."));
	}
	else
	{
		// Mensaje de advertencia si no se encuentra un PlayerController válido
		// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("No se encontró un PlayerController válido para actualizar interacciones."));
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
		return;
	}

	UAbilitySystemComponent* AbilitySystem = GetAbilitySystemComponentFromActorInfo();
	if (AbilitySystem)
	{
		const FInteractionOption& InteractionOption = CurrentOptions[0];

		// Verificar si la opción de interacción soporta interacción mantenida
		if (!InteractionOption.bSupportsHoldInteraction)
		{
			// Si no soporta interacción mantenida, se cancela la operación
			return;
		}

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
			AbilityHandle = InteractionOption.TargetInteractionAbilityHandle;
		}

		// Activar la habilidad usando la etiqueta de evento de interacción mantenida
		const bool bSuccess = InteractionOption.TargetAbilitySystem->TriggerAbilityFromGameplayEvent(
			AbilityHandle,
			&ActorInfo,
			TAG_Ability_Interaction_Hold_Activate,
			&Payload,
			*InteractionOption.TargetAbilitySystem
		);
		
		// Notificar que se completó la interacción mantenida
		OnHoldInteractionComplete.Broadcast();
	}
}

void UGameplayAbility_Interact::OnInteractionButtonPressed()
{
	if (CurrentOptions.Num() == 0)
	{
		return;
	}
	
	bIsInteractionButtonHeld = true;
	InteractionStartTime = GetWorld()->GetTimeSeconds();
	
	// Configurar un temporizador para detectar si el botón se mantiene presionado durante el tiempo requerido
	GetWorld()->GetTimerManager().SetTimer(
		HoldInteractionTimerHandle,
		this,
		&UGameplayAbility_Interact::OnHoldInteractionTimeElapsed,
		HoldInteractionTime,
		false
	);
}

void UGameplayAbility_Interact::OnInteractionButtonReleased()
{
	if (!bIsInteractionButtonHeld)
	{
		return;
	}
	
	bIsInteractionButtonHeld = false;
	
	// Cancelar el temporizador si existe
	if (GetWorld()->GetTimerManager().IsTimerActive(HoldInteractionTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(HoldInteractionTimerHandle);
		
		// Si el botón se soltó antes del tiempo de interacción mantenida, considerarlo como una pulsación rápida
		float CurrentTime = GetWorld()->GetTimeSeconds();
		float HoldTime = CurrentTime - InteractionStartTime;
		
		if (HoldTime < HoldInteractionTime)
		{
			// Llamar a la interacción de pulsación rápida
			TriggerInteraction();
			
			// Llamar a InteractPressScan después de la interacción para restaurar el estado
			// y prepararse para la siguiente interacción
			// Nota: Esto debe implementarse en el blueprint conectando la salida de este nodo
			// a la entrada del nodo InteractPressScan
		}
	}
}

void UGameplayAbility_Interact::OnHoldInteractionTimeElapsed()
{
	if (bIsInteractionButtonHeld)
	{
		// El botón se ha mantenido presionado durante el tiempo requerido
		// Llamar a la interacción mantenida
		TriggerHoldInteraction();
		
		// Nota: Después de llamar a TriggerHoldInteraction, 
		// es necesario llamar a InteractPressScan en el blueprint
		// para restaurar el estado y prepararse para la siguiente interacción
		// Esto se logra escuchando al delegado OnHoldInteractionComplete
	}
}
