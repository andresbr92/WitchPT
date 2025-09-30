// Fill out your copyright notice in the Description page of Project Settings.


#include "Behaviour/Int_GameplayBehaviour_InteractionWithAbility.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Behaviour/Int_GameplayBehaviourConfig_InteractionWithAbility.h"
#include "Components/Int_InteractionSystemComponent.h"

bool UInt_GameplayBehaviour_InteractionWithAbility::Trigger(AActor& Avatar, const UGameplayBehaviorConfig* Config,
                                                            AActor* SmartObjectOwner)
{
	bTransientIsTriggering = true;
	bTransientIsActive = false;
	TransientAvatar = &Avatar;
	TransientSmartObjectOwner = SmartObjectOwner;

	UInt_InteractionSystemComponent* InteractionSystem = Avatar.FindComponentByClass<UInt_InteractionSystemComponent>();

	if (!InteractionSystem)
	{
		UE_LOG(LogTemp, Error, TEXT("Interaction system not found"));
		return false;
	}

	UAbilitySystemComponent* Asc = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(&Avatar);
	if (!Asc)
	{
		UE_LOG(LogTemp, Error, TEXT("Ability system component not found"));
		return false;
	}

	TSubclassOf<UGameplayAbility> AbilityClass{nullptr};
	int32 AbilityLevel = 0;
	if (!CheckValidAbilitySetting(Config, AbilityClass, AbilityLevel))
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid ability setting"));
		return false;
	}

	if (FGameplayAbilitySpec* Handle = Asc->FindAbilitySpecFromClass(AbilityClass))
	{
		UE_LOG(LogTemp, Error, TEXT("Ability already exists"));
		return false;
	}

	GrantedAbilityClass = AbilityClass;

	AbilityEndedDelegateHandle = Asc->OnAbilityEnded.AddUObject(this, &ThisClass::OnAbilityEndedCallback);

	//Ability trigger by event when activation polciy=ServerInitied won't work.
	AbilitySpecHandle = Asc->K2_GiveAbilityAndActivateOnce(AbilityClass, AbilityLevel);

	if (!AbilitySpecHandle.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to give and activate ability"));
		return false;
	}

	// Special case: behavior already interrupted
	if (bBehaviorWasInterrupted && AbilitySpecHandle.IsValid() && !bAbilityEnded)
	{
		Asc->ClearAbility(AbilitySpecHandle);
		return false;
	}

	if (AbilitySpecHandle.IsValid() && bAbilityEnded)
	{
		UE_LOG(LogTemp, Warning, TEXT("Instantly executed interaction ability:%s,handle%s"), *AbilityClass->GetName(), *AbilitySpecHandle.ToString());
		
		EndBehavior(Avatar, false);
		return false;
	}

	UE_LOG(LogTemp, Warning, TEXT("Granted and activate interaction ability:%s,handle%s"), *AbilityClass->GetName(), *AbilitySpecHandle.ToString());
	
	UE_LOG(LogTemp, Warning, TEXT("Interaction begins with ability:%s"), *AbilityClass->GetName());
	bTransientIsTriggering = false;
	bTransientIsActive = true;
	return bTransientIsActive;
}

void UInt_GameplayBehaviour_InteractionWithAbility::EndBehavior(AActor& Avatar, const bool bInterrupted)
{
	
	

	// clear ability stuff.
	if (UAbilitySystemComponent* Asc = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(&Avatar))
	{
		UE_LOG(LogTemp, Warning, TEXT("EndBehavior called, bInterrupted: %s, bTransientIsActive: %s, bAbilityEnded: %s"), bInterrupted ? TEXT("true") : TEXT("false"),
			bTransientIsActive ? TEXT("true") : TEXT("false"), bAbilityEnded ? TEXT("true") : TEXT("false"));
		if (AbilityEndedDelegateHandle.IsValid())
		{
			Asc->OnAbilityEnded.Remove(AbilityEndedDelegateHandle);
			AbilityEndedDelegateHandle.Reset();
		}

		// Special case: behavior interrupting active ability, so cancel ability.
		if (bInterrupted && bTransientIsActive && !bAbilityEnded && AbilitySpecHandle.IsValid())
		{
			if (const FGameplayAbilitySpec* Spec = Asc->FindAbilitySpecFromHandle(AbilitySpecHandle))
			{
				UE_LOG(LogTemp, Warning, TEXT("Cancelling ability %s due to behavior interruption"), *Spec->Ability->GetName());
				Asc->CancelAbilityHandle(AbilitySpecHandle);
			}
		}

		if (bInterrupted && !bTransientIsActive && AbilitySpecHandle.IsValid())
		{
			Asc->ClearAbility(AbilitySpecHandle);
		}
	}

	Super::EndBehavior(Avatar, bInterrupted);

	bBehaviorWasInterrupted = bInterrupted;
}

bool UInt_GameplayBehaviour_InteractionWithAbility::CheckValidAbilitySetting(const UGameplayBehaviorConfig* Config,
	TSubclassOf<UGameplayAbility>& OutAbilityClass, int32& OutAbilityLevel)
{
	// Ability class validation.
	const UInt_GameplayBehaviourConfig_InteractionWithAbility* InteractionConfig = Cast<const UInt_GameplayBehaviourConfig_InteractionWithAbility>(Config);
	if (!InteractionConfig)
	{
		UE_LOG(LogTemp, Warning, TEXT("InteractionConfig is not valid"));
		return false;
	}

	const TSubclassOf<UGameplayAbility> AbilityClass = InteractionConfig->AbilityToGrant.LoadSynchronous();
	if (!AbilityClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilityClass is not valid"));
		return false;
	}
	OutAbilityClass = AbilityClass;
	OutAbilityLevel = InteractionConfig->AbilityLevel;
	return true;
	
}

void UInt_GameplayBehaviour_InteractionWithAbility::OnAbilityEndedCallback(const FAbilityEndedData& EndedData)
{
	if (bAbilityEnded)
	{
		return;
	}
	// check for ability granted by this behavior.
	if (EndedData.AbilitySpecHandle == AbilitySpecHandle || EndedData.AbilityThatEnded->GetClass() == GrantedAbilityClass)
	{
		bAbilityEnded = true;
		bAbilityWasCancelled = EndedData.bWasCancelled;

		// Special case: behavior already active and abilities ended, ending behavior normally.
		if (!bTransientIsTriggering && bTransientIsActive)
		{
			UE_LOG(LogTemp, Warning, TEXT("AbilityEndedCallback triggered"));
			
			EndBehavior(*GetAvatar(), false);
		}
	}
}
