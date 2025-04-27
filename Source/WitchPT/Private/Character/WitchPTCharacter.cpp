// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/WitchPTCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/WitchPTAbilitySystemComponent.h"
#include "AbilitySystem/WitchPTAttributeSet.h"
#include "Player/WitchPTPlayerState.h"


// Sets default values
AWitchPTCharacter::AWitchPTCharacter(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AWitchPTCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init ability actor info for the Server
	InitAbilityActorInfo();

	// Solo conceder habilidades si AbilitySystemComponent fue inicializado correctamente
	if (AbilitySystemComponent)
	{
		GrantStartupAbilities();
		AddStartupEffects();
	}
}

void AWitchPTCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	// Init ability actor info for the Client
	InitAbilityActorInfo();
	AddStartupEffects();
}

void AWitchPTCharacter::InitAbilityActorInfo()
{
	Super::InitAbilityActorInfo();
	AWitchPTPlayerState* WitchPtPlayerState = GetPlayerState<AWitchPTPlayerState>();
	
	if (!WitchPtPlayerState)
	{
		return;
	}
	
	WitchPtPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(WitchPtPlayerState, this);

	AbilitySystemComponent = Cast<UWitchPTAbilitySystemComponent>(WitchPtPlayerState->GetAbilitySystemComponent());
	if (!AbilitySystemComponent)
	{
		return;
	}
	
	// TODO: delegates on ASC
	// We call this function after de ability actor info to bind al functions to the ASC delegates
	
	// Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoHaveBeenSet();
	AttributeSet = Cast<UWitchPTAttributeSet>( WitchPtPlayerState->GetAttributeSet());
	// We can call this ONLY in the server because attributes are marked as replicated. But is ok doing it here.
	InitializeDefaultAttributes();
}



