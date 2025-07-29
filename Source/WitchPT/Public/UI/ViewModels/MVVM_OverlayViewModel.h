// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WitchPT_MVVMViewModelBase.h"
#include "MVVM_OverlayViewModel.generated.h"

class UMVVMView;
/**
 * 
 */
UCLASS()
class WITCHPT_API UMVVM_OverlayViewModel : public UWitchPT_MVVMViewModelBase
{
	GENERATED_BODY()
	
public:
	virtual void BindCallbacksToDependencies() override;
	virtual void BroadcastInitialValues() override;
	
private:

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess))
	int32 Health = 0;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess))
	int32 MaxHealth = 0;
	
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess))
	int32 Age = 0;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess))
	int32 MaxAge = 0;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess))
	int32 Stamina = 0;
	
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess))
	int32 MaxStamina = 0;



public:
	void SetHealth(int32 NewHealth);
	void SetMaxHealth(int32 NewMaxHealth);
	void SetAge(int32 NewAge);
	void SetMaxAge(int32 NewMaxAge);
	void SetStamina(int32 NewStamina);
	void SetMaxStamina(int32 NewMaxStamina);

	int32 GetHealth() const { return Health; }
	int32 GetMaxHealth() const { return MaxHealth; }
	int32 GetAge() const { return Age; }
	int32 GetMaxAge() const { return MaxAge; }
	int32 GetStamina() const { return Stamina; }
	int32 GetMaxStamina() const { return MaxStamina; }



	
	
public:
	UFUNCTION(BlueprintPure, FieldNotify)
	float GetHealthPercent() const
	{
		if (MaxHealth != 0)
		{
			return (float) Health / (float) MaxHealth;
		}
		else
			return 0;
	}
	
	UFUNCTION(BlueprintPure, FieldNotify)
	float GetAgePercent() const
	{
		if (MaxAge != 0)
		{
			return (float) Age / (float) MaxAge;
		}
		else
			return 0;
	}

	UFUNCTION(BlueprintPure, FieldNotify)
	float GetStaminaPercent() const
	{
		if (MaxStamina != 0)
		{
			return (float) Stamina / (float) MaxStamina;
		}
		else
			return 0;
	}
};
