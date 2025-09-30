// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayBehavior.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Int_GameplayBehaviour_InteractionWithAbility.generated.h"

/**
 * 
 */
UCLASS(DisplayName="Gameplay Behaviour Interaction (AndresD3v)", NotBlueprintable)
class ANDRESD3V_INTERACTIONSYSTEM_API UInt_GameplayBehaviour_InteractionWithAbility : public UGameplayBehavior
{
	GENERATED_BODY()
public:

	
public:
	/**
	 * Triggers the interaction behavior.
	 * @param InAvatar The avatar actor.
	 * @param Config The behavior config.
	 * @param SmartObjectOwner The smart object owner. 
	 * @return True if the behavior was triggered successfully, false otherwise.
	 */
	virtual bool Trigger(AActor& InAvatar, const UGameplayBehaviorConfig* Config, AActor* SmartObjectOwner) override;

	/**
	 * Ends the behavior.
	 * @param Avatar The avatar actor.
	 * @param bInterrupted Whether the behavior was interrupted.
	 */
	virtual void EndBehavior(AActor& Avatar, const bool bInterrupted) override;

	/**
	 * Checks the validity of the ability settings.
	 * 检查技能设置的有效性。
	 * @param Config The behavior config.
	 * @param OutAbilityClass The ability class (output).
	 * @param OutAbilityLevel The ability level (output).
	 * @return True if the settings are valid, false otherwise.
	 */
	bool CheckValidAbilitySetting(const UGameplayBehaviorConfig* Config, TSubclassOf<UGameplayAbility>& OutAbilityClass, int32& OutAbilityLevel);

	/**
	 * The ability class granted for the interaction.
	 */
	UPROPERTY()
	TSubclassOf<UGameplayAbility> GrantedAbilityClass{nullptr};

	/**
	 * Handle for the granted ability spec.
	 */
	FGameplayAbilitySpecHandle AbilitySpecHandle;

	/**
	 * Indicates if the behavior was interrupted.
	 */
	bool bBehaviorWasInterrupted = false;

	/**
	 * Indicates if the ability has ended.
	 */
	bool bAbilityEnded = false;

	/**
	 * Indicates if the ability was cancelled.
	 */
	bool bAbilityWasCancelled = false;

	/**
	 * Delegate handle for ability end notification.
	 */
	FDelegateHandle AbilityEndedDelegateHandle;

	/**
	 * Called when the ability ends.
	 * @param EndedData The ability end data.
	 */
	virtual void OnAbilityEndedCallback(const FAbilityEndedData& EndedData);
};
