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

void UWitchPTAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			// print each ability
			
			if (AbilitySpec.Ability && (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag)))
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, AbilitySpec.Ability->GetName());
				InputPressedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.AddUnique(AbilitySpec.Handle);
			}
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
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag)))
			{
				InputReleasedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.Remove(AbilitySpec.Handle);
			}
		}
	}
}

void UWitchPTAbilitySystemComponent::ProcessAbilityInput(float DeltaTime, bool bGamePaused)
{
		static TArray<FGameplayAbilitySpecHandle> AbilitiesToActivate;
	AbilitiesToActivate.Reset();
	//
	// Process all abilities that activate when the input is held.
	//
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputHeldSpecHandles)
	{
		
		
		if (const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability && !AbilitySpec->IsActive())
			{
				const UWitchPTGameplayAbility* PTAbilityBase = Cast<UWitchPTGameplayAbility>(AbilitySpec->Ability);
				if (PTAbilityBase && PTAbilityBase->GetActivationPolicy() == EPrototypeAbilityActivationPolicy::WhileInputActive)
				{
					// --- MODIFICACION AQUI ---
					// Solo añadir si la política es WhileInputActive y NO es la nueva On Triggered Event
					if (PTAbilityBase && PTAbilityBase->GetActivationPolicy() == EPrototypeAbilityActivationPolicy::WhileInputActive)
					{
						AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
					}
				}
			}
		}
	}
	//
	// Process all abilities that had their input pressed this frame.
	//
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputPressedSpecHandles)
	{
		
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = true;

				if (AbilitySpec->IsActive())
				{
					
					
					// Ability is active so pass along the input event.
					AbilitySpecInputPressed(*AbilitySpec);
				}
				else
				{
					const UWitchPTGameplayAbility* PTAbilityBase = Cast<UWitchPTGameplayAbility>(AbilitySpec->Ability);

					if (PTAbilityBase && PTAbilityBase->GetActivationPolicy() == EPrototypeAbilityActivationPolicy::OnInputTriggered)
					{
						if (PTAbilityBase && PTAbilityBase->GetActivationPolicy() == EPrototypeAbilityActivationPolicy::OnInputTriggered)
						{
							AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
						}
					}
				}
			}
		}
	}
	//
	// Try to activate all the abilities that are from presses and holds.
	// We do it all at once so that held inputs don't activate the ability
	// and then also send a input event to the ability because of the press.
	//
	for (const FGameplayAbilitySpecHandle& AbilitySpecHandle : AbilitiesToActivate)
	{
		TryActivateAbility(AbilitySpecHandle);
	}
	//
	// Process all abilities that had their input released this frame.
	//
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputReleasedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = false;

				if (AbilitySpec->IsActive())
				{
					// Ability is active so pass along the input event.
					AbilitySpecInputReleased(*AbilitySpec);
				}
			}
		}
	}

	//
	// Clear the cached ability handles.
	//
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
	
}

void UWitchPTAbilitySystemComponent::AbilitySpecInputPressed(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputPressed(Spec);
	// We don't support UGameplayAbility::bReplicateInputDirectly.
	// Use replicated events instead so that the WaitInputPress ability task works.
	if (Spec.IsActive())
	{
		PRAGMA_DISABLE_DEPRECATION_WARNINGS
		const UGameplayAbility* Instance = Spec.GetPrimaryInstance();
		FPredictionKey OriginalPredictionKey = Instance ? Instance->GetCurrentActivationInfo().GetActivationPredictionKey() : Spec.ActivationInfo.GetActivationPredictionKey();
		PRAGMA_ENABLE_DEPRECATION_WARNINGS

		// Invoke the InputPressed event. This is not replicated here. If someone is listening, they may replicate the InputPressed event to the server.
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle, OriginalPredictionKey);
	}
	
}

void UWitchPTAbilitySystemComponent::AbilitySpecInputReleased(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputReleased(Spec);
	// We don't support UGameplayAbility::bReplicateInputDirectly.
	// Use replicated events instead so that the WaitInputRelease ability task works.
	if (Spec.IsActive())
	{
		PRAGMA_DISABLE_DEPRECATION_WARNINGS
		const UGameplayAbility* Instance = Spec.GetPrimaryInstance();
		FPredictionKey OriginalPredictionKey = Instance ? Instance->GetCurrentActivationInfo().GetActivationPredictionKey() : Spec.ActivationInfo.GetActivationPredictionKey();
		PRAGMA_ENABLE_DEPRECATION_WARNINGS

		// Invoke the InputReleased event. This is not replicated here. If someone is listening, they may replicate the InputReleased event to the server.
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, Spec.Handle, OriginalPredictionKey);
	}
}
