// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WitchPTWidgetController.h"
#include "GenericContainerWidgetController.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class WITCHPT_API UGenericContainerWidgetController : public UWitchPTWidgetController
{
	GENERATED_BODY()
public:
	void SetControllerPackage(const TMap<TSubclassOf<UWitchPTWidgetController>, UWitchPTWidgetController*>& InControllers);

	UFUNCTION(BlueprintCallable, Category = "Controllers", meta = (DeterminesOutputType = "ControllerClass"))
	UWitchPTWidgetController* GetControllerByClass(TSubclassOf<UWitchPTWidgetController> ControllerClass);

private:
	UPROPERTY()
	TMap<TSubclassOf<UWitchPTWidgetController>, UWitchPTWidgetController*> ControllerPackage;
};
