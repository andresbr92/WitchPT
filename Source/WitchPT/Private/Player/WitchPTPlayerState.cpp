// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/WitchPTPlayerState.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/WitchPTAbilitySystemComponent.h"
#include "AbilitySystem/WitchPTAttributeSet.h"
#include "Item/RitualPosition.h"

AWitchPTPlayerState::AWitchPTPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UWitchPTAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UWitchPTAttributeSet>("AttributeSet");


	SetNetUpdateFrequency(100.f);
}

UAbilitySystemComponent* AWitchPTPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AWitchPTPlayerState::SetRitualAltarInteractingWith(ARitualPosition* RitualPosition)
{
	if (RitualPosition)
	{
		RitualPositionInteractingWith = RitualPosition;
		RitualAltarInteractingWith = RitualPosition->GetRitualAltar();
	}
}
