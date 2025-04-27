// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayAbilities/WitchPTGameplayAbility.h"

#include "AbilitySystemComponent.h"

UWitchPTGameplayAbility::UWitchPTGameplayAbility(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateNo;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ClientOrServer;
	ActivationPolicy = EPrototypeAbilityActivationPolicy::OnInputTriggered;
}

void UWitchPTGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo,
                                            const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	TryActivateAbilityOnSpawn(ActorInfo, Spec);
}

void UWitchPTGameplayAbility::TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilitySpec& Spec) const
{
	if (ActorInfo && !Spec.IsActive() && (ActivationPolicy == EPrototypeAbilityActivationPolicy::OnSpawn))
	{
		UAbilitySystemComponent* AbilitySystem = ActorInfo->AbilitySystemComponent.Get();
		AbilitySystem->TryActivateAbility(Spec.Handle);
	}
}
