// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetControllers/WitchPTWidgetController.h"

void UWitchPTWidgetController::SetContextualObject(UObject* ContextualObject)
{
	ISetContextualObjectInterface::SetContextualObject(ContextualObject);
}

void UWitchPTWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void UWitchPTWidgetController::BroadcastInitialValues()
{
}

void UWitchPTWidgetController::BindCallbacksToDependencies()
{
}
