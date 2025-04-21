// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Components/WitchPTCharacterMovementComponent.h"


// Sets default values for this component's properties
UWitchPTCharacterMovementComponent::UWitchPTCharacterMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

float UWitchPTCharacterMovementComponent::GetMaxSpeed() const
{
	return Super::GetMaxSpeed();
}

void UWitchPTCharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);
}

class FNetworkPredictionData_Client* UWitchPTCharacterMovementComponent::GetPredictionData_Client() const
{
	return Super::GetPredictionData_Client();
}


void UWitchPTCharacterMovementComponent::StartSprinting()
{
	RequestToStartSprinting = true;
}

void UWitchPTCharacterMovementComponent::StopSprinting()
{
	RequestToStartSprinting = false;
}

void UWitchPTCharacterMovementComponent::StartAimDownSights()
{
	RequestToStartADS = true;
}

void UWitchPTCharacterMovementComponent::StopAimDownSights()
{
	RequestToStartADS = false;
}
