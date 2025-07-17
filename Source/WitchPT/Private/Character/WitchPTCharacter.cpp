// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/WitchPTCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/WitchPTAbilitySystemComponent.h"
#include "AbilitySystem/WitchPTAttributeSet.h"
#include "Character/Components/WitchPTMechanicComponent.h"
#include "Equipment/WitchPTEquipmentManagerComponent.h"
#include "Item/RitualAltar.h"
#include "Item/RitualPosition.h"
#include "Player/WitchPTPlayerController.h"
#include "Player/WitchPTPlayerState.h"
#include "UI/HUD/WitchPTHUD.h"
#include "WitchPT/WitchPT.h"


// Sets default values
AWitchPTCharacter::AWitchPTCharacter(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Initialize the MechanicComponent
	MechanicComponent = CreateDefaultSubobject<UWitchPTMechanicComponent>(TEXT("MechanicComponent"));
	// Initialize the EquipmentManagerComponent
	EquipmentManagerComponent = CreateDefaultSubobject<UWitchPTEquipmentManagerComponent>(TEXT("EquipmentManagerComponent"));
	
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
	
	AbilitySystemComponent->AbilityActorInfoHaveBeenSet();
	AttributeSet = Cast<UWitchPTAttributeSet>( WitchPtPlayerState->GetAttributeSet());
	

	
	/*
	* Here we initialize the Overlay because we have all the data we need: the player controller, player state, ability system component and attribute set.
	* Also, the HUD is accessible from the player controller.
	*/
	if (AWitchPTPlayerController* WitchPT_PC = Cast<AWitchPTPlayerController>(GetController()))
	{
		if (AWitchPTHUD* WitchPT_HUD = Cast<AWitchPTHUD>(WitchPT_PC->GetHUD()))
		{
			// WitchPT_HUD->InitOverlay(WitchPT_PC, WitchPtPlayerState, AbilitySystemComponent, AttributeSet);
			// WitchPT_HUD->InitRitualWidget(WitchPT_PC, WitchPtPlayerState, AbilitySystemComponent, AttributeSet);
			// WitchPT_HUD->InitInventoryWidget(WitchPT_PC, WitchPtPlayerState, AbilitySystemComponent, AttributeSet);
			// WitchPT_HUD->InitCauldronWidget(WitchPT_PC, WitchPtPlayerState, AbilitySystemComponent, AttributeSet);
			// WitchPT_HUD->InitQuickBarWidget(WitchPT_PC, WitchPtPlayerState, AbilitySystemComponent, AttributeSet);
			// WitchPT_HUD->InitAllWidgets(WitchPT_PC, WitchPtPlayerState, AbilitySystemComponent, AttributeSet);
			
		}
	}
	InitializeDefaultAttributes();
}





