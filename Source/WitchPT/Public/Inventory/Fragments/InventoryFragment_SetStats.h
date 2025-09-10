// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "WitchPTInventoryItemFragment.h"
#include "InventoryFragment_SetStats.generated.h"

/**
 * 
 */
class UWitchPTInventoryItemInstance;
class UObject;
struct FGameplayTag;

UCLASS()
class WITCHPT_API UInventoryFragment_SetStats : public UWitchPTInventoryItemFragment
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TMap<FGameplayTag, int32> InitialItemStats;

public:
	virtual void OnInstanceCreated(UWitchPTInventoryItemInstance* Instance) const override;

	int32 GetItemStatByTag(FGameplayTag Tag) const;
};
