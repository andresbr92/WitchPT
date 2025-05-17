// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RitualFunctionLibrary.generated.h"

class URitualWidgetController;
/**
 * 
 */
UCLASS()
class WITCHPT_API URitualFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	UFUNCTION(BlueprintPure, Category="Inventory")
	static URitualWidgetController* SetInventoryWidgetController(const UObject* WorldContextObject);
};
