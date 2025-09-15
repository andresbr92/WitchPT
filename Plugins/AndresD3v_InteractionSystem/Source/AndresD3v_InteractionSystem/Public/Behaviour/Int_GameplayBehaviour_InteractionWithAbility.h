// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayBehavior.h"
#include "Int_GameplayBehaviour_InteractionWithAbility.generated.h"

/**
 * 
 */
UCLASS(DisplayName="Gameplay Behaviour Interaction (AndresD3v)", NotBlueprintable)
class ANDRESD3V_INTERACTIONSYSTEM_API UInt_GameplayBehaviour_InteractionWithAbility : public UGameplayBehavior
{
	GENERATED_BODY()
public:

	
	virtual bool Trigger(AActor& Avatar, const UGameplayBehaviorConfig* Config = nullptr, AActor* SmartObjectOwner = nullptr) override;
	
	virtual void EndBehavior(AActor& Avatar, const bool bInterrupted = false) override;
};
