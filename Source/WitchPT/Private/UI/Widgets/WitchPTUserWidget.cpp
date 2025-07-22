// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/WitchPTUserWidget.h"

void UWitchPTUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	// SetVisibility(ESlateVisibility::Collapsed);
}

void UWitchPTUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	OnWidgetControllerSet();
}

void UWitchPTUserWidget::SetPoolingInfo(const TSubclassOf<UUserWidget>& InPoolKey, UObject* InContextObject)
{
	PoolKey = InPoolKey;
	if (InContextObject != nullptr)
	{
		LastContextObject = InContextObject;
	}
}
