// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/WitchPTInventoryItemFragment.h"
#include "WitchPTInventoryFragment_Details.generated.h"

/**
 * 
 */
UCLASS()
class WITCHPT_API UWitchPTInventoryFragment_Details : public UWitchPTInventoryItemFragment
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Title;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Description;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UUserWidget> IconWidget;
	
};
