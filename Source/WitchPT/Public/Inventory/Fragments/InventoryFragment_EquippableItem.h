// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WitchPTInventoryItemFragment.h"
#include "InventoryFragment_EquippableItem.generated.h"

class UWitchPTEquipmentDefinition;
/**
 * 
 */
UCLASS()
class WITCHPT_API UInventoryFragment_EquippableItem : public UWitchPTInventoryItemFragment
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category=Equipment)
	TSubclassOf<UWitchPTEquipmentDefinition> EquipmentDefinition;
};
