// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayBehaviorConfig.h"
#include "Int_GameplayBehaviourConfig_InteractionWithAbility.generated.h"

class UGameplayAbility;
/**
 * 
 */
UCLASS(DisplayName= "Gameplay Behaviour Config Interaction (AndresD3v)")
class ANDRESD3V_INTERACTIONSYSTEM_API
	UInt_GameplayBehaviourConfig_InteractionWithAbility : public UGameplayBehaviorConfig
{
	GENERATED_BODY()
public:
	UInt_GameplayBehaviourConfig_InteractionWithAbility();

	/**
	 * The ability to grant and activate when interaction begins.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interaction")
	TSoftClassPtr<UGameplayAbility> AbilityToGrant;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interaction")
	TSoftClassPtr<UGameplayAbility> DurationAbilityToGrant;

	

	/**
	 * Ability level
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interaction")
	int32 AbilityLevel{0};
};
