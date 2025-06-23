// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/WitchPTHUD.h"

#include "Inventory/WitchPTInventoryManagerComponent.h"
#include "Player/WitchPTPlayerController.h"
#include "UI/WidgetControllers/CauldronWidgetController.h"
#include "UI/WidgetControllers/InventoryWidgetController.h"
#include "UI/WidgetControllers/OverlayWidgetController.h"
#include "UI/WidgetControllers/RitualWidgetController.h"
#include "UI/Widgets/WitchPTUserWidget.h"
#include "UI/Widgets/Inventory/RitualUserWidget.h"

UOverlayWidgetController* AWitchPTHUD::SetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies();
	}
	return OverlayWidgetController;
}

UCauldronWidgetController* AWitchPTHUD::SetCauldronWidgetController(const FWidgetControllerParams& WCParams)
{
	if (CauldronWidgetController == nullptr)
	{
		CauldronWidgetController = NewObject<UCauldronWidgetController>(this, CauldronWidgetControllerClass);
		CauldronWidgetController->SetWidgetControllerParams(WCParams);
		// CauldronWidgetController->BindCallbacksToDependencies();
	}
	return CauldronWidgetController;
}

UInventoryWidgetController* AWitchPTHUD::SetInventoryWidgetController(const FWidgetControllerParams& WCParams)
{
	if (InventoryWidgetController == nullptr)
	{
		InventoryWidgetController = NewObject<UInventoryWidgetController>(this, InventoryWidgetControllerClass);
		InventoryWidgetController->SetWidgetControllerParams(WCParams);
		InventoryWidgetController->BindCallbacksToDependencies();
	}
	return InventoryWidgetController;
}

URitualWidgetController* AWitchPTHUD::SetRitualWidgetController(const FWidgetControllerParams& WCParams)
{
	if (RitualWidgetController == nullptr)
	{
		RitualWidgetController = NewObject<URitualWidgetController>(this, RitualWidgetControllerClass);
		RitualWidgetController->SetWidgetControllerParams(WCParams);
	}
	return RitualWidgetController;
}

void AWitchPTHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized, please fill out BP_WitchPTHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class uninitialized, please fill out BP_WitchPTHUD"));

	// Initialize Overlay Widget
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UWitchPTUserWidget>(Widget);

	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UOverlayWidgetController* WidgetController = SetOverlayWidgetController(WidgetControllerParams);

	OverlayWidget->SetWidgetController(WidgetController);
	WidgetController->BroadcastInitialValues();
	Widget->AddToViewport();

	// Initialize Ritual Widget (but keep it hidden)
	if (RitualWidgetClass)
	{
		UUserWidget* RitualWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), RitualWidgetClass);
		RitualWidget = Cast<UWitchPTUserWidget>(RitualWidgetInstance);
		
		if (RitualWidget)
		{
			// Create and set the ritual widget controller
			URitualWidgetController* RitualController = SetRitualWidgetController(WidgetControllerParams);
			RitualWidget->SetWidgetController(RitualController);
			
			// Add to viewport but set visibility to collapsed
			RitualWidgetInstance->AddToViewport();
			RitualWidgetInstance->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void AWitchPTHUD::ShowRitualWidget(class ARitualAltar* RitualAltar)
{
	if (RitualWidget && RitualWidgetController)
	{
		// Set the ritual altar reference in the controller
		// Note: SetRitualAltar() already calls BindCallbacksToDependencies() and BroadcastInitialValues()
		RitualWidgetController->SetRitualAltar(RitualAltar);
		
		// Use enhanced widget show method if available
		URitualUserWidget* RitualUserWidget = Cast<URitualUserWidget>(RitualWidget);
		if (RitualUserWidget)
		{
			RitualUserWidget->ShowForRitual(RitualAltar);
		}
		
		// Show the widget
		RitualWidget->SetVisibility(ESlateVisibility::Visible);
		
		// Set input mode to allow UI interaction
		APlayerController* PC = GetOwningPlayerController();
		if (PC)
		{
			FInputModeGameAndUI InputMode;
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			InputMode.SetHideCursorDuringCapture(false);
			PC->SetInputMode(InputMode);
			PC->bShowMouseCursor = true;
		}
	}
}

void AWitchPTHUD::HideRitualWidget()
{
	if (RitualWidget)
	{
		// Use enhanced widget hide method if available
		URitualUserWidget* RitualUserWidget = Cast<URitualUserWidget>(RitualWidget);
		if (RitualUserWidget)
		{
			RitualUserWidget->HideFromRitual();
		}
		
		// Hide the widget
		RitualWidget->SetVisibility(ESlateVisibility::Collapsed);
		
		// Clear the ritual altar reference in the controller
		if (RitualWidgetController)
		{
			RitualWidgetController->SetRitualAltar(nullptr);
		}
		
		// Restore game-only input mode
		APlayerController* PC = GetOwningPlayerController();
		if (PC)
		{
			PC->SetInputMode(FInputModeGameOnly());
			PC->bShowMouseCursor = false;
		}
	}
}



