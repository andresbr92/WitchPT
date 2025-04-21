// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/WitchPTAbilitySystemComponent.h"

#include "AbilitySystem/GameplayAbilities/WitchPTGameplayAbility.h"


void UWitchPTAbilitySystemComponent::GrantStartupAbilities(
	const TArray<TSubclassOf<UGameplayAbility>>& AbilitiesToGrant)
{
	if (!AbilitiesToGrant.IsEmpty())
	{
		for (const TSubclassOf<UGameplayAbility> AbilityClass: AbilitiesToGrant)
		{
			if (!AbilityClass)
			{
				continue;
			}
			
			// Obtener la clase de habilidad para acceder a la propiedad StartupInputTag
			UWitchPTGameplayAbility* AbilityCDO = AbilityClass->GetDefaultObject<UWitchPTGameplayAbility>();
			if (!AbilityCDO)
			{
				continue;
			}
			
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
			// Source of the ability is the ASC
			AbilitySpec.SourceObject = this;
			// Add the startup input tag to the ability spec
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilityCDO->StartupInputTag);
			// Give the ability to the ASC
			FGameplayAbilitySpecHandle AbilitySpecHandle = GiveAbility(AbilitySpec);
			// Store the handle of the ability
			GrantedAbilities.Add(AbilitySpecHandle);
		}
	}
}

void UWitchPTAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	for (FGameplayAbilitySpec& AbilitySpec: GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UWitchPTAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	for (FGameplayAbilitySpec& AbilitySpec: GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}
