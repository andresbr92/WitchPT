// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InventoryFunctionLibrary.generated.h"

class UInventoryWidgetController;
/**
 * 
 */
UCLASS()
class WITCHPT_API UInventoryFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category="Inventory")
	static UInventoryWidgetController* SetInventoryWidgetController(const UObject* WorldContextObject);
};
