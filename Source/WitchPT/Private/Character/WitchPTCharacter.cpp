// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/WitchPTCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/WitchPTAbilitySystemComponent.h"
#include "AbilitySystem/WitchPTAttributeSet.h"
#include "Item/RitualAltar.h"
#include "Item/RitualPosition.h"
#include "Player/WitchPTPlayerState.h"
#include "WitchPT/WitchPT.h"


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

void AWitchPTCharacter::HandleStartRitualRequest_Implementation(ACharacter* RequestingCharacter)
{
	Server_HandleStartRitualRequest(RequestingCharacter);
}

void AWitchPTCharacter::HandlePlayerInput_Implementation(ACharacter* InputCharacter, const FGameplayTag& InputTag)
{
	Server_HandlePlayerInput(InputCharacter, InputTag);
}

void AWitchPTCharacter::HandleSetRitualPosition_Implementation(ARitualPosition* InRitualPosition)
{
	if (InRitualPosition)
	{
		this->RitualPosition = InRitualPosition;
	}
}

bool AWitchPTCharacter::IsRitualPositionOccupied_Implementation() const
{
	if (RitualPosition)
	{
		return RitualPosition->IsOccupied();
	}
	return false;
}

void AWitchPTCharacter::SetRitualPositionOccupied_Implementation(ACharacter* OccupyingCharacter)
{
	if (RitualPosition)
	{
		RitualPosition->Server_SetOccupied(OccupyingCharacter);
		// Server_SetRitualPositionOccupied(OccupyingCharacter, RitualPosition);
	}
}

void AWitchPTCharacter::HandleSetCauldronAltar_Implementation(ACauldronAltar* InCauldronAltar)
{
	if (InCauldronAltar)
	{
		CauldronAltar = InCauldronAltar;
	}
}

void AWitchPTCharacter::HandleStartBrewingPotion_Implementation(ACharacter* InteractingCharacter)
{
	if (InteractingCharacter && CauldronAltar)
	{
		Server_HandleStartBrewingPotion(InteractingCharacter);
	}
}

void AWitchPTCharacter::HandleStartCarryCauldron_Implementation(ACharacter* InteractingCharacter)
{
	if (InteractingCharacter && CauldronAltar)
	{
		Server_HandleStartCarryCauldron(InteractingCharacter);
	}
}

void AWitchPTCharacter::HandleStopBrewingPotion_Implementation(ACharacter* InteractingCharacter)
{
	if ( CauldronAltar)
	{
		Server_HandleStartBrewingPotion(InteractingCharacter);
	}
	
}

void AWitchPTCharacter::HandleStopCarryCauldron_Implementation(ACharacter* InteractingCharacter)
{
	Server_HandleStopCarryCauldron(InteractingCharacter);

}

void AWitchPTCharacter::Server_HandleStopCarryCauldron_Implementation(ACharacter* InteractingCharacter)
{
	if ( CauldronAltar)
	{
		// CauldronAltar->StopBrewingPotion
	}
}

void AWitchPTCharacter::Server_HandleStopBrewingPotion_Implementation(ACharacter* InteractingCharacter)
{
	if (CauldronAltar)
	{
		// CauldronAltar->
	}
}

void AWitchPTCharacter::Server_HandleStartCarryCauldron_Implementation(ACharacter* InteractingCharacter)
{
	if (InteractingCharacter && CauldronAltar)
	{
		CauldronAltar->StartCarryCauldron(InteractingCharacter);
	}
}

void AWitchPTCharacter::Server_HandleStartBrewingPotion_Implementation(ACharacter* InteractingCharacter)
{
	if (InteractingCharacter && CauldronAltar)
	{
		CauldronAltar->StartBrewingPotion(InteractingCharacter);
	}
}


void AWitchPTCharacter::Server_HandlePlayerInput_Implementation(ACharacter* InputCharacter, const FGameplayTag& InputTag)
{
	if (RitualPosition)
	{
		RitualPosition->GetRitualAltar()->HandlePlayerInput(InputCharacter, InputTag);
	}
}

void AWitchPTCharacter::Server_HandleStartRitualRequest_Implementation(ACharacter* RequestingCharacter)
{
	if (RitualPosition)
	{
		RitualPosition->GetRitualAltar()->StartRitual(RequestingCharacter);
	}
	
}


void AWitchPTCharacter::Server_SetRitualPositionOccupied_Implementation(ACharacter* Player, ARitualPosition* Position)
{
	if (Player && Position)
	{
		RitualPosition->GetRitualAltar()->OccupyPosition(Player, Position);
	}
}




