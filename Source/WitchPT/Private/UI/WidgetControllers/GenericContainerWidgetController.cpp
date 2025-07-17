// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetControllers/GenericContainerWidgetController.h"

void UGenericContainerWidgetController::SetControllerPackage(
	const TMap<TSubclassOf<UWitchPTWidgetController>, UWitchPTWidgetController*>& InControllers)
{
	ControllerPackage = InControllers;
}

UWitchPTWidgetController* UGenericContainerWidgetController::GetControllerByClass(
	TSubclassOf<UWitchPTWidgetController> ControllerClass)
{
	if (ControllerClass && ControllerPackage.Contains(ControllerClass))
	{
		return ControllerPackage[ControllerClass];
	}
	return nullptr;
}
