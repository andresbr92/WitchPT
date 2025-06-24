// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/CauldronUserWidget.h"

void UCauldronUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	// Input mode is now managed by HUD's ShowCauldronWithInventory/HideCauldronWithInventory methods
	// Don't set input mode here as this is called during widget creation, not visibility changes
}

FReply UCauldronUserWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}
