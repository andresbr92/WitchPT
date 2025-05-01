// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Components/WitchPTCharacterMovementComponent.h"

#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "Character/WitchPTCharacterBase.h"



UWitchPTCharacterMovementComponent::UWitchPTCharacterMovementComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;


}

float UWitchPTCharacterMovementComponent::GetMaxSpeed() const
{
	
	AWitchPTCharacterBase* Owner = Cast<AWitchPTCharacterBase>(GetOwner());

	if (!Owner)
	{
		
		return Super::GetMaxSpeed();
	}

	
	
	if (Owner->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Character.Status.Blocked"))))
	{
		
		return 0.0f;
	}

	float FinalSpeed = Owner->GetMoveSpeed();
	if (RequestToStartSprinting)
	{
		FinalSpeed = Owner->GetMoveSpeed() * Owner->GetSpeedMultiplier();
		
	}
	// else if (RequestToStartCrouching)
	// {
	// 	// FinalSpeed = Owner->GetMoveSpeed() * CrouchSpeedMultiplier;
	// 	// UE_LOG(LogTemp, Warning, TEXT("Character %s is Crouching - Speed: %f"), *GetOwner()->GetName(), FinalSpeed);
	// }

	
	return FinalSpeed;
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
