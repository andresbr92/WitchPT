// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "WitchPTInventoryItemFragment.h"
#include "WitchPTInventoryFragment_UIDetails.generated.h"

/**
 * 
 */
UCLASS()
class WITCHPT_API UWitchPTInventoryFragment_UIDetails : public UWitchPTInventoryItemFragment
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Title;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Description;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UUserWidget> IconWidget;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag TierTag;
	
	
};
