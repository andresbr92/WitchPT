// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/WitchPTAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "WitchPT/WitchPT.h"

void UWitchPTAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UWitchPTAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWitchPTAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always)

	DOREPLIFETIME_CONDITION_NOTIFY(UWitchPTAttributeSet, Mana, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UWitchPTAttributeSet, MaxMana, COND_None, REPNOTIFY_Always)

	DOREPLIFETIME_CONDITION_NOTIFY(UWitchPTAttributeSet, Stamina, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UWitchPTAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always)
	
	DOREPLIFETIME_CONDITION_NOTIFY(UWitchPTAttributeSet, Age, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UWitchPTAttributeSet, MaxAge, COND_None, REPNOTIFY_Always)
	
	DOREPLIFETIME_CONDITION_NOTIFY(UWitchPTAttributeSet, Strength, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UWitchPTAttributeSet, MaxStrength, COND_None, REPNOTIFY_Always)
	
	DOREPLIFETIME_CONDITION_NOTIFY(UWitchPTAttributeSet, SpeedMultiplier, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UWitchPTAttributeSet, MaxSpeedMultiplier, COND_None, REPNOTIFY_Always)

	


}

UWitchPTAttributeSet::UWitchPTAttributeSet()
{
}

void UWitchPTAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
}

void UWitchPTAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		CONSOLE_LOG("Health value: %f", GetHealth());
	}
}

void UWitchPTAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWitchPTAttributeSet, Health, OldHealth);
}

void UWitchPTAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWitchPTAttributeSet, Mana, OldMana);
}
void UWitchPTAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWitchPTAttributeSet, MaxHealth, OldMaxHealth);
}

void UWitchPTAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWitchPTAttributeSet, MaxMana, OldMaxMana);
}

void UWitchPTAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWitchPTAttributeSet, Stamina, OldStamina);
}

void UWitchPTAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWitchPTAttributeSet, MaxStamina, OldMaxStamina);
}

void UWitchPTAttributeSet::OnRep_Age(const FGameplayAttributeData& OldAge) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWitchPTAttributeSet, Age, OldAge);
}

void UWitchPTAttributeSet::OnRep_MaxAge(const FGameplayAttributeData& OldMaxAge) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWitchPTAttributeSet, MaxAge, OldMaxAge);
}

void UWitchPTAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWitchPTAttributeSet, Strength, OldStrength);
}

void UWitchPTAttributeSet::OnRep_MaxStrength(const FGameplayAttributeData& OldMaxStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWitchPTAttributeSet, MaxStrength, OldMaxStrength);
}

void UWitchPTAttributeSet::OnRep_SpeedMultiplier(const FGameplayAttributeData& OldSpeedMultiplier) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWitchPTAttributeSet, SpeedMultiplier, OldSpeedMultiplier);
}

void UWitchPTAttributeSet::OnRep_MaxSpeedMultiplier(const FGameplayAttributeData& OldMaxSpeedMultiplier) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWitchPTAttributeSet, MaxSpeedMultiplier, OldMaxSpeedMultiplier);
}

