// Fill out your copyright notice in the Description page of Project Settings.


#include "Behaviour/Int_GameplayBehaviour_InteractionWithAbility.h"

bool UInt_GameplayBehaviour_InteractionWithAbility::Trigger(AActor& Avatar, const UGameplayBehaviorConfig* Config,
	AActor* SmartObjectOwner)
{
	return Super::Trigger(Avatar, Config, SmartObjectOwner);
}

void UInt_GameplayBehaviour_InteractionWithAbility::EndBehavior(AActor& Avatar, const bool bInterrupted)
{
	Super::EndBehavior(Avatar, bInterrupted);
}
