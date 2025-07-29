// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModels/WitchPT_MVVMViewModelBase.h"

void UWitchPT_MVVMViewModelBase::SetViewModelParams(const FViewModelParams& Params)
{
	PlayerController = Params.PlayerController;
	PlayerState = Params.PlayerState;
	AbilitySystemComponent = Params.AbilitySystemComponent;
	AttributeSet = Params.AttributeSet;
}

void UWitchPT_MVVMViewModelBase::BindCallbacksToDependencies()
{
}

void UWitchPT_MVVMViewModelBase::BroadcastInitialValues()
{
}

void UWitchPT_MVVMViewModelBase::UnbindCallbacksFromDependencies()
{
}
