// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/WitchPTUserWidget.h"

void UWitchPTUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	OnWidgetControllerSet();
}
