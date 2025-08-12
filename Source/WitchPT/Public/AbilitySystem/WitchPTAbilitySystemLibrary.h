// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WitchPTAbilitySystemLibrary.generated.h"

class IMechanicsInterface;
/**
 * 
 */
UCLASS()
class WITCHPT_API UWitchPTAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category = "Ability|Mechanics", meta = (DisplayName = "Get Mechanics Interface", DeterminesOutputType = "IMechanicsInterface"))
	static TScriptInterface<IMechanicsInterface> GetMechanicsInterface(const AActor* Actor);
};
