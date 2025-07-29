// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModels/MVVM_OverlayViewModel.h"

#include "AbilitySystem/WitchPTAttributeSet.h"


void UMVVM_OverlayViewModel::BindCallbacksToDependencies()
{
    
    if (const UWitchPTAttributeSet* WitchPT_AS = Cast<UWitchPTAttributeSet>(AttributeSet))
    {

        AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(WitchPT_AS->GetHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
		{
    		SetHealth(Data.NewValue);
		});
    	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(WitchPT_AS->GetMaxHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
    	{
    		SetMaxHealth(Data.NewValue);
    	});


    	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(WitchPT_AS->GetAgeAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
		{
    		SetAge(Data.NewValue);
		});
    	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(WitchPT_AS->GetMaxAgeAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
    	{
    		SetMaxAge(Data.NewValue);
    	});

        AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(WitchPT_AS->GetStaminaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
		{
    		SetStamina(Data.NewValue);
		});
    	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(WitchPT_AS->GetMaxStaminaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
    	{
    		SetMaxStamina(Data.NewValue);
    	});
    }
  
	
}

void UMVVM_OverlayViewModel::BroadcastInitialValues()
{
	if (const UWitchPTAttributeSet* WitchPT_AS = Cast<UWitchPTAttributeSet>(AttributeSet))
	{
		SetAge(WitchPT_AS->GetAge());
		SetMaxAge(WitchPT_AS->GetMaxAge());
		SetHealth(WitchPT_AS->GetHealth());
		SetMaxHealth(WitchPT_AS->GetMaxHealth());
		SetStamina(WitchPT_AS->GetStamina());
		SetMaxStamina(WitchPT_AS->GetMaxStamina());
	}
	
}
void UMVVM_OverlayViewModel::SetHealth(int32 NewHealth)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(Health, NewHealth))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetHealthPercent);
	}
}

void UMVVM_OverlayViewModel::SetMaxHealth(int32 NewMaxHealth)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(MaxHealth, NewMaxHealth))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetHealthPercent);
	}
}



void UMVVM_OverlayViewModel::SetAge(int32 NewAge)
{
	
	if (UE_MVVM_SET_PROPERTY_VALUE(Age, NewAge))
	{
		UE_LOG(LogTemp, Log, TEXT("Age changed to: %d"), NewAge);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetAgePercent);
	}
}

void UMVVM_OverlayViewModel::SetMaxAge(int32 NewMaxAge)
{
	UE_LOG(LogTemp, Log, TEXT("Setting MaxAge to: %d"), NewMaxAge);
	if(UE_MVVM_SET_PROPERTY_VALUE(MaxAge, NewMaxAge))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetAgePercent);
	}
}

void UMVVM_OverlayViewModel::SetStamina(int32 NewStamina)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(Stamina, NewStamina))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetStaminaPercent);
	}
}


void UMVVM_OverlayViewModel::SetMaxStamina(int32 NewMaxStamina)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(MaxStamina, NewMaxStamina))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetStaminaPercent);
	}
}


