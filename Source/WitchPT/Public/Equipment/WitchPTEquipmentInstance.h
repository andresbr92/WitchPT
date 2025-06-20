// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "WitchPTEquipmentInstance.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class WITCHPT_API UWitchPTEquipmentInstance : public UObject
{
	GENERATED_BODY()
public:
	UWitchPTEquipmentInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	

	// Equipment lifecycle
	virtual void OnEquipped();
	virtual void OnUnequipped();

	UFUNCTION(BlueprintPure, Category=Equipment)
	APawn* GetPawn() const;
protected:
	UFUNCTION(BlueprintImplementableEvent, Category=Equipment)
	void K2_OnEquipped();
    
	UFUNCTION(BlueprintImplementableEvent, Category=Equipment)
	void K2_OnUnequipped();

	
};
