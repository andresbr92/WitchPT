// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/RitualAltar.h"

#include "Net/UnrealNetwork.h"
#include "WitchPT/WitchPT.h"


// Sets default values
ARitualAltar::ARitualAltar()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true; // Keep Tick for now, might be needed for timers later

	bReplicates = true;

	
	

}

void ARitualAltar::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARitualAltar, InputSequence);
	DOREPLIFETIME(ARitualAltar, CurrentSequenceIndex);
	DOREPLIFETIME(ARitualAltar, ParticipatingPlayers);
	DOREPLIFETIME(ARitualAltar, CurrentInputTimer);
	DOREPLIFETIME(ARitualAltar, CorruptionAmount);
	DOREPLIFETIME(ARitualAltar, CorruptionIncreasePerFail);
	DOREPLIFETIME(ARitualAltar, BaseInputTimeWindow);
	DOREPLIFETIME(ARitualAltar, DifficultyScalingMultiplier);
}

bool ARitualAltar::StartRitual(ACharacter* Character)
{
	Server_StartRitual(Character);
	return true;
}

void ARitualAltar::Server_StartRitual_Implementation(ACharacter* InitiatingPlayer)
{
	CONSOLE_LOG("Started ritual")
}

void ARitualAltar::Multicast_OnInputSuccess_Implementation(ACharacter* Player)
{
}

void ARitualAltar::Multicast_OnInputFailed_Implementation(ACharacter* Player)
{
}

void ARitualAltar::Multicast_OnRitualSucceeded_Implementation()
{
}

void ARitualAltar::Multicast_OnRitualCatastrophicFail_Implementation()
{
}

void ARitualAltar::Multicast_OnRitualStateChanged_Implementation(ERitualState NewState)
{
}

void ARitualAltar::Server_OccupyPosition_Implementation(ACharacter* Player, ARitualPosition* Position)
{
}

void ARitualAltar::Server_HandlePlayerInput_Implementation(ACharacter* Character, const FGameplayTag& InputTag)
{
}