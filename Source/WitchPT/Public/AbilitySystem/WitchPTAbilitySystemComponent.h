// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "WitchPTAbilitySystemComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class WITCHPT_API UWitchPTAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void GrantStartupAbilities(const TArray<TSubclassOf<UGameplayAbility>>& AbilitiesToGrant);
	
	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	
	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);
	
	bool bCharacterAbilitiesGiven = false;
	bool bStartupEffectsApplied = false;

	TArray<FGameplayAbilitySpecHandle> GrantedAbilities;

	// Handles to abilities that had their input pressed this frame.
	UPROPERTY(VisibleAnywhere, Category = "Ability System")
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;

	// Handles to abilities that had their input released this frame.
	UPROPERTY(VisibleAnywhere, Category = "Ability System")
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;

	
	// Handles to abilities that have their input held.
	UPROPERTY(VisibleAnywhere, Category = "Ability System")
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;

protected:
	virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override;
	virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;

};
