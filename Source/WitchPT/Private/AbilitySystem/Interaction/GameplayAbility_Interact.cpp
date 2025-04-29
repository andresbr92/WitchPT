// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Interaction/GameplayAbility_Interact.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/Interaction/AbilityTask_GrantNearbyInteraction.h"
#include "AbilitySystem/Interaction/InteractionStatics.h"
#include "NativeGameplayTags.h"
#include "AbilitySystem/WitchPTAbilitySystemComponent.h"
#include "AbilitySystem/Interaction/IInteractableTarget.h"
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Ability_Interaction_Activate, "Ability.Interaction.Activate");
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
	}
}
