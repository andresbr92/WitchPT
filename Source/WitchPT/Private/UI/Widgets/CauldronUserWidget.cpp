// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/CauldronUserWidget.h"

void UCauldronUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	FInputModeGameAndUI InputMode;
	// set the input mode to the player controller
	
	if (APlayerController* OwningController = GetOwningPlayer())
	{
		OwningController->SetInputMode(InputMode);
		OwningController->SetShowMouseCursor(true);
	}
	
}

FReply UCauldronUserWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}
