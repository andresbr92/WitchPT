// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WitchPT_MVVMViewModelBase.h"
#include "MVVM_OverlayViewModel.generated.h"

/**
 * 
 */
UCLASS()
class WITCHPT_API UMVVM_OverlayViewModel : public UWitchPT_MVVMViewModelBase
{
	GENERATED_BODY()
private:
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess))
	int32 CurrentHealth;
		
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess))
	int32 MaxHealth;
public:
	void SetCurrentHealth(int32 NewCurrentHealth)
	{
		if (UE_MVVM_SET_PROPERTY_VALUE(CurrentHealth, NewCurrentHealth))
		{
			UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetHealthPercent);
		}
	}
	void SetMaxHealth(int32 NewMaxHealth)
	{
		if (UE_MVVM_SET_PROPERTY_VALUE(MaxHealth, NewMaxHealth))
		{
			UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetHealthPercent);
		}
	}
	int32 GetCurrentHealth() const
	{
		return CurrentHealth;
	}
	int32 GetMaxHealth() const
	{
		return MaxHealth;
	}
public:
	UFUNCTION(BlueprintPure, FieldNotify)
	float GetHealthPercent() const
	{
		if (MaxHealth != 0)
		{
			return (float) CurrentHealth / (float) MaxHealth;
		}
		else
			return 0;
	}
};
