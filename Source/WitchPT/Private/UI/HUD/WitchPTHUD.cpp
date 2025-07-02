// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/WitchPTHUD.h"

#include "Inventory/WitchPTInventoryManagerComponent.h"
#include "Player/WitchPTPlayerController.h"
#include "UI/WidgetControllers/CauldronWidgetController.h"
#include "UI/WidgetControllers/InventoryWidgetController.h"
#include "UI/WidgetControllers/OverlayWidgetController.h"
#include "UI/WidgetControllers/QuickBarWidgetController.h"
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

UQuickBarWidgetController* AWitchPTHUD::SetQuickBarWidgetController(const FWidgetControllerParams& WCParams)
{
	if (QuickBarWidgetController == nullptr)
	{
		QuickBarWidgetController = NewObject<UQuickBarWidgetController>(this, QuickBarWidgetControllerClass);
		QuickBarWidgetController->SetWidgetControllerParams(WCParams);
		QuickBarWidgetController->BindCallbacksToDependencies();
		
	}
	return QuickBarWidgetController;
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
	InitRitualWidget(PC, PS, ASC, AS);
	// Initialize Inventory Widget (but keep it hidden)
	InitInventoryWidget(PC, PS, ASC, AS);
	// Initialize Cauldron Widget (but keep it hidden)
	InitCauldronWidget(PC, PS, ASC, AS);
	// Initialize Quick Bar Widget 
	// InitQuickBarWidget(PC, PS, ASC, AS);
	
}

void AWitchPTHUD::InitRitualWidget(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC,
	UAttributeSet* AS)
{
	if (RitualWidgetClass)
	{
		
		UUserWidget* RitualWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), RitualWidgetClass);
		RitualWidget = Cast<UWitchPTUserWidget>(RitualWidgetInstance);
		
		if (RitualWidget)
		{
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			// Create and set the ritual widget controller
			URitualWidgetController* RitualController = SetRitualWidgetController(WidgetControllerParams);
			RitualWidget->SetWidgetController(RitualController);
			
			// Add to viewport but set visibility to collapsed
			RitualWidgetInstance->AddToViewport();
			RitualWidgetInstance->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void AWitchPTHUD::InitInventoryWidget(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC,
	UAttributeSet* AS)
{
	if (InventoryWidgetClass && !InventoryWidget)
	{
		UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), InventoryWidgetClass);
		InventoryWidget = Cast<UWitchPTUserWidget>(Widget);
        
		if (InventoryWidget)
		{
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			UInventoryWidgetController* Controller = SetInventoryWidgetController(WidgetControllerParams);
			InventoryWidget->SetWidgetController(Controller);
			Widget->AddToViewport();
			Widget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void AWitchPTHUD::InitCauldronWidget(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	if (CauldronWidgetClass && !CauldronWidget)
	{
		UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), CauldronWidgetClass);
		CauldronWidget = Cast<UWitchPTUserWidget>(Widget);
        
		if (CauldronWidget)
		{
			const FWidgetControllerParams WCParams(PC, PS, ASC, AS);
			UCauldronWidgetController* Controller = SetCauldronWidgetController(WCParams);
			CauldronWidget->SetWidgetController(Controller);
			Widget->AddToViewport();
			Widget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void AWitchPTHUD::InitQuickBarWidget(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC,
	UAttributeSet* AS)
{
	if (QuickBarUserWidgetClass && !QuickBarUserWidget)
	{
		UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), QuickBarUserWidgetClass);
		QuickBarUserWidget = Cast<UWitchPTUserWidget>(Widget);
		if (QuickBarUserWidget)
		{
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			UQuickBarWidgetController* Controller = SetQuickBarWidgetController(WidgetControllerParams);
			QuickBarUserWidget->SetWidgetController(Controller);
			
			Widget->AddToViewport();
		}
	}
	
	
}

void AWitchPTHUD::ShowOverlayWidget()
{
	OverlayWidget->SetVisibility(ESlateVisibility::Visible);
}

void AWitchPTHUD::HideOverlayWidget()
{
	OverlayWidget->SetVisibility(ESlateVisibility::Collapsed);
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

void AWitchPTHUD::ShowInventoryWidget()
{
	if (InventoryWidget)
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Visible);
        
		APlayerController* PC = GetOwningPlayerController();
		if (PC)
		{
			FInputModeGameAndUI InputMode;
			PC->SetInputMode(InputMode);
			PC->SetShowMouseCursor(true);
		}
	}
	
}

void AWitchPTHUD::HideInventoryWidget()
{
	if (InventoryWidget)
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
        
		APlayerController* PC = GetOwningPlayerController();
		if (PC)
		{
			PC->SetInputMode(FInputModeGameOnly());
			PC->SetShowMouseCursor(false);
		}
	}
}

void AWitchPTHUD::ShowCauldronWithInventory(class ACauldronAltar* CauldronAltar)
{
	if (CauldronWidget && InventoryWidget && CauldronWidgetController)
	{
		// Set up cauldron altar reference
		CauldronWidgetController->SetCauldronAltar(CauldronAltar);
		CauldronWidgetController->BindCallbacksToDependencies();
		CauldronWidgetController->BroadcastInitialValues();
        
		// Show both widgets
		CauldronWidget->SetVisibility(ESlateVisibility::Visible);
        
		bCauldronWidgetVisible = true;
        
		// Set input mode
		APlayerController* PC = GetOwningPlayerController();
		if (PC)
		{
			FInputModeGameAndUI InputMode;
			PC->SetInputMode(InputMode);
			PC->SetShowMouseCursor(true);
		}
	}
}

void AWitchPTHUD::HideCauldronWithInventory()
{
	if (CauldronWidget && InventoryWidget)
	{
		CauldronWidget->SetVisibility(ESlateVisibility::Collapsed);
		InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
        
		bCauldronWidgetVisible = false;
        
		// Clear cauldron altar reference
		if (CauldronWidgetController)
		{
			CauldronWidgetController->SetCauldronAltar(nullptr);
			CauldronWidgetController->UnbindCallbacksFromDependencies();
		}
        
		// Restore game input mode
		APlayerController* PC = GetOwningPlayerController();
		if (PC)
		{
			PC->SetInputMode(FInputModeGameOnly());
			PC->SetShowMouseCursor(false);
		}
	}
}



