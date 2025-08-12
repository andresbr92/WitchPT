// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GameplayAbilities/WitchPTGameplayAbility.h"
#include "GA_Cauldron_Crafting.generated.h"

class ACauldronAltar;
class ABaseInteractableAltar;
/**
 * 
 */
UCLASS()
class WITCHPT_API UGA_Cauldron_Crafting : public UWitchPTGameplayAbility
{
	GENERATED_BODY()
public:
	UFUNCTION()
	void OnCancelParticipatingInCauldron(FGameplayEventData Payload);
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	UFUNCTION()
	void OnCharacterPositioned(FGameplayEventData Payload);
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
protected:
	

	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> CauldronViewWidgetClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> StandardBlockedEffectClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> CauldronBrewingEffectClass;
	
	TArray<FActiveGameplayEffectHandle> ActiveEffectsHandles;
	const ACauldronAltar* CurrentCauldronAltar;
	
};
