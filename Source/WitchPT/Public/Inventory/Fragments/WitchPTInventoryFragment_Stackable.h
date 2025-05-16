// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WitchPTInventoryItemFragment.h"
#include "WitchPTInventoryFragment_Stackable.generated.h"

/**
 * 
 */
UCLASS()
class WITCHPT_API UWitchPTInventoryFragment_Stackable : public UWitchPTInventoryItemFragment
{
	GENERATED_BODY()
public:
	
	int32 GetMaxStackSize() const { return MaxStackSize; }
	int32 GetStackCount() const { return StackCount; }
	void SetStackCount(int32 Count) { StackCount = Count; }

private:

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 MaxStackSize{1};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 StackCount{1};
};
