// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Inventory/WitchPTInventoryItemFragment.h"
#include "WitchPTInventoryFragment_Stats.generated.h"
class UWitchPTInventoryItemInstance;
class UObject;
struct FGameplayTag;
/**
 * 
 */
UCLASS()
class WITCHPT_API UWitchPTInventoryFragment_Stats : public UWitchPTInventoryItemFragment
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TMap<FGameplayTag, int32> InitialItemStats;
	
	virtual void OnInstanceCreated(UWitchPTInventoryItemInstance* Instance) const override;

	int32 GetItemStatByTag(FGameplayTag Tag) const;
};
