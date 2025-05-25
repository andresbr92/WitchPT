// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UI/WidgetControllers/CauldronWidgetController.h"
#include "RitualFunctionLibrary.generated.h"

class UWitchPTWidgetController;
class URitualWidgetController;
/**
 * 
 */
UCLASS()
class WITCHPT_API URitualFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(Category="Ritual")
	static URitualWidgetController* SetRitualWidgetController(const UObject* WorldContextObject);
	UFUNCTION(Category="Cauldron")
	static UCauldronWidgetController* SetCauldronWidgetController(const UObject* WorldContextObject);
};
