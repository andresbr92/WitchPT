// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WidgetControllerFunctionLibrary.generated.h"

class UInventoryWidgetController;
class UQuickBarWidgetController;
class URitualWidgetController;
class UCauldronWidgetController;
class UOverlayWidgetController;

/**
 * 
 */
UCLASS()
class WITCHPT_API UWidgetControllerFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	public:
	UFUNCTION(BlueprintPure, Category="Inventory", meta=(WorldContext="WorldContextObject"))
	static UInventoryWidgetController* SetInventoryWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category="QuickBar", meta=(WorldContext="WorldContextObject"))
	static UQuickBarWidgetController* SetQuickBarWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category="Ritual", meta=(WorldContext="WorldContextObject"))
	static URitualWidgetController* SetRitualWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category="Cauldron", meta=(WorldContext="WorldContextObject"))
	static UCauldronWidgetController* SetCauldronWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category="Overlay", meta=(WorldContext="WorldContextObject"))
	static UOverlayWidgetController* SetOverlayWidgetController(const UObject* WorldContextObject);

};
