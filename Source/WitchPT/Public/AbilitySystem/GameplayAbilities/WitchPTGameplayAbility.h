// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "WitchPTGameplayAbility.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EPrototypeAbilityActivationPolicy: uint8
{
	// Try to activate the ability when the input is triggered.
	OnInputTriggered,

	// Continually try to activate the ability while the input is active.
	WhileInputActive,

	// Try to activate the ability when an avatar is assigned.
	OnSpawn,
	
	OnTriggeredEvent
};
UCLASS()
class WITCHPT_API UWitchPTGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UWitchPTGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	FGameplayTag StartupInputTag;
	EPrototypeAbilityActivationPolicy GetActivationPolicy() const { return ActivationPolicy; }
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WitchPT |Ability Activation")
	EPrototypeAbilityActivationPolicy ActivationPolicy;

	void TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) const;
};
