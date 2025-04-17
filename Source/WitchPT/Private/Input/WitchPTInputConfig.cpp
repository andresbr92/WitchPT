// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/WitchPTInputConfig.h"

const UInputAction* UWitchPTInputConfig::GetAbilityByInputTag(FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FWitchPTInputAction& Pair: AbilityInputActions)
	{
		if (InputTag == Pair.InputTag)
		{
			return Pair.InputAction;
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Cant find AbilityInputAction for InputTag [%s] on InputConfig [%s]"), *InputTag.ToString(), *GetNameSafe(this) )
	}
	return nullptr;
	
}
