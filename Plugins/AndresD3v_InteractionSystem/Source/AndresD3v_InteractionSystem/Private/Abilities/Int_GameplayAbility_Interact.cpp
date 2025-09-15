// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/Int_GameplayAbility_Interact.h"

#include "Int_InteractionDefinition.h"
#include "SmartObjectBlueprintFunctionLibrary.h"
#include "Components/Int_InteractionSystemComponent.h"

UInt_GameplayAbility_Interact::UInt_GameplayAbility_Interact()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateYes;
}

void UInt_GameplayAbility_Interact::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	InteractionSystem = UInt_InteractionSystemComponent::GetInteractionSystemComponent(ActorInfo->AvatarActor.Get());
	if (!InteractionSystem)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}
	// TODO: IMPLEMENT EVENT
	
	// InteractionSystem->OnInteractableActorChangedEvent.AddDynamic...
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UInt_GameplayAbility_Interact::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// TODO: REMOVE EVENT
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UInt_GameplayAbility_Interact::TryClaimInteraction(int32 Index, FSmartObjectClaimHandle& ClaimedHandle)
{
	USmartObjectSubsystem* Subsystem = USmartObjectSubsystem::GetCurrent(GetWorld());

	check(Subsystem!=nullptr)
	const TArray<FInt_InteractionOption>& InteractionInstances = InteractionSystem->GetInteractionOptions();
	if (!InteractionInstances.IsValidIndex(Index))
	{
		UE_LOG(LogTemp, Error, TEXT("Interaction at index(%d) not exist!!"), Index);
		return false;
	}

	if (InteractionInstances[Index].Definition == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Interaction at index(%d) has invalid definition!"), Index);
		return false;
	}

	if (InteractionInstances[Index].SlotState != ESmartObjectSlotState::Free)
	{
		UE_LOG(LogTemp, Error, TEXT("Interaction(%s) was Claimed/Occupied!"), *InteractionInstances[Index].Definition->Text.ToString());
		return false;
	}

	const FInt_InteractionOption& CurrentOption = InteractionInstances[Index];

	FSmartObjectClaimHandle NewlyClaimedHandle = USmartObjectBlueprintFunctionLibrary::MarkSmartObjectSlotAsClaimed(GetWorld(), CurrentOption.RequestResult.SlotHandle, GetAvatarActorFromActorInfo());

	// A valid claimed handle can point to an object that is no longer part of the simulation
	if (!Subsystem->IsClaimedSmartObjectValid(NewlyClaimedHandle))
	{
		UE_LOG(LogTemp, Error, TEXT("Interaction(%s) refers to an object that is no longer available.!"), *InteractionInstances[Index].Definition->Text.ToString());
		return false;
	}

	ClaimedHandle = NewlyClaimedHandle;
	return true;
}

void UInt_GameplayAbility_Interact::OnInteractActorChanged_Implementation(AActor* OldActor, AActor* NewActor)
{
}
