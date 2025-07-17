// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/WitchPTHUD.h"

#include "Inventory/WitchPTInventoryManagerComponent.h"
#include "Item/CauldronAltar.h"
#include "Player/WitchPTPlayerController.h"
#include "UI/WidgetControllers/CauldronWidgetController.h"
#include "UI/WidgetControllers/InventoryWidgetController.h"
#include "UI/WidgetControllers/OverlayWidgetController.h"
#include "UI/WidgetControllers/QuickBarWidgetController.h"
#include "UI/WidgetControllers/RitualWidgetController.h"
#include "UI/Widgets/QuickBarUserWidget.h"
#include "UI/Widgets/WitchPTPrimaryLayout.h"
#include "UI/Widgets/WitchPTUserWidget.h"
#include "UI/Widgets/Inventory/InventoryUserWidget.h"
#include "UI/Widgets/Inventory/RitualUserWidget.h"

void AWitchPTHUD::BeginPlay()
{
	Super::BeginPlay();
	if (PrimaryLayout == nullptr)
	{
		UUserWidget* PrimaryLayoutInstance = CreateWidget<UUserWidget>(GetOwningPlayerController(), PrimaryLayoutClass);
		PrimaryLayout = Cast<UWitchPTPrimaryLayout>(PrimaryLayoutInstance);
		PrimaryLayout->AddToViewport();
		
	}
}

TMap<TSubclassOf<UWitchPTWidgetController>, UWitchPTWidgetController*> AWitchPTHUD::CreateWidgetsControllers(
	const TArray<TSubclassOf<UWitchPTWidgetController>>& ControllerClasses, const FWidgetControllerParams& WCParams,
	UObject* ContextObject)
{
	TMap<TSubclassOf<UWitchPTWidgetController>, UWitchPTWidgetController*> ResultPackage;
	for (TSubclassOf ControllerClass : ControllerClasses)
	{
		if (!ControllerClass) continue;
		
		TObjectPtr<UWitchPTWidgetController>* FoundController = ControllerCache.Find(ControllerClass);
		UWitchPTWidgetController* ControllerInstance = nullptr;

		if (FoundController && *FoundController)
		{
			ControllerInstance = *FoundController;
		}
		else
		{
			ControllerInstance = NewObject<UWitchPTWidgetController>(this, ControllerClass);
			ControllerCache.Add(ControllerClass, ControllerInstance);
		}

		
		ControllerInstance->SetContextualObject(ContextObject);
		ControllerInstance->SetWidgetControllerParams(WCParams);
		ControllerInstance->BindCallbacksToDependencies();
		ControllerInstance->BroadcastInitialValues();
	
		ResultPackage.Add(ControllerClass, ControllerInstance);
	}

	return ResultPackage;
}

UWitchPTUserWidget* AWitchPTHUD::GetMenuWidgetByClass(TSubclassOf<UWitchPTUserWidget> WidgetClass)
{
	if (WidgetClass)
	{
		for (UUserWidget* Widget : GameMenuWidgets)
		{
			if (Widget && Widget->IsA(WidgetClass))
			{
				return Cast<UWitchPTUserWidget>(Widget);
			}
		}
	}
	return nullptr;
}

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

void AWitchPTHUD::InitAllWidgets(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	// Initialize Overlay Widget
	if (OverlayWidgetClass && OverlayWidgetControllerClass)
	{
		UUserWidget* OverlayWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
		OverlayWidget = Cast<UWitchPTUserWidget>(OverlayWidgetInstance);

		const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
		UOverlayWidgetController* WidgetController = SetOverlayWidgetController(WidgetControllerParams);

		OverlayWidget->SetWidgetController(WidgetController);
		WidgetController->BroadcastInitialValues();
		GameMenuWidgets.Add(OverlayWidgetInstance);
	}

	// Initialize Ritual Widget
	if (RitualWidgetClass)
	{
		UUserWidget* RitualWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), RitualWidgetClass);
		RitualWidget = Cast<UWitchPTUserWidget>(RitualWidgetInstance);
		
		if (RitualWidget)
		{
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			URitualWidgetController* RitualController = SetRitualWidgetController(WidgetControllerParams);
			RitualWidget->SetWidgetController(RitualController);
			GameMenuWidgets.Add(RitualWidgetInstance);
		}
	}

	// Initialize Inventory Widget
	if (InventoryWidgetClass && !InventoryWidget)
	{
		UUserWidget* InventoryWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), InventoryWidgetClass);
		InventoryWidget = Cast<UWitchPTUserWidget>(InventoryWidgetInstance);
        
		if (InventoryWidget)
		{
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			UInventoryWidgetController* Controller = SetInventoryWidgetController(WidgetControllerParams);
			InventoryWidget->SetWidgetController(Controller);
			GameMenuWidgets.Add(InventoryWidgetInstance);
		}
	}

	// Initialize Cauldron Widget
	if (CauldronWidgetClass && !CauldronWidget)
	{
		UUserWidget* CauldronWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), CauldronWidgetClass);
		CauldronWidget = Cast<UWitchPTUserWidget>(CauldronWidgetInstance);
        
		if (CauldronWidget)
		{
			const FWidgetControllerParams WCParams(PC, PS, ASC, AS);
			UCauldronWidgetController* Controller = SetCauldronWidgetController(WCParams);
			CauldronWidget->SetWidgetController(Controller);
			GameMenuWidgets.Add(CauldronWidgetInstance);
		}
	}

	// Initialize QuickBar Widget
	if (QuickBarUserWidgetClass && !QuickBarUserWidget)
	{
		UUserWidget* QuickBarWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), QuickBarUserWidgetClass);
		QuickBarUserWidget = Cast<UWitchPTUserWidget>(QuickBarWidgetInstance);
		if (QuickBarUserWidget)
		{
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			UQuickBarWidgetController* Controller = SetQuickBarWidgetController(WidgetControllerParams);
			QuickBarUserWidget->SetWidgetController(Controller);
			GameMenuWidgets.Add(QuickBarWidgetInstance);
		}
	}
}

void AWitchPTHUD::ToggleGameMenu(TSubclassOf<UWitchPTUserWidget> WidgetClass)
{
	// Find the widget by class
	if (WidgetClass)
	{
		UWitchPTUserWidget* WidgetToToggle = nullptr;
		for (UUserWidget* Widget : GameMenuWidgets)
		{
			if (Widget->IsA(WidgetClass))
			{
				WidgetToToggle = Cast<UWitchPTUserWidget>(Widget);
				break;
			}
		}
		if (!WidgetToToggle) return;
		const bool bIsTargetWidgetAlreadyVisible = WidgetToToggle->IsVisible();
		// Hide all game menu widgets first
		for (auto& MenuWidget : GameMenuWidgets)
		{
			if (MenuWidget && MenuWidget->IsVisible())
			{
				MenuWidget->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
		APlayerController* PC = GetOwningPlayerController();
		
		if (bIsTargetWidgetAlreadyVisible)
		{
			if (PC)
			{
				PC->SetInputMode(FInputModeGameOnly());
				PC->bShowMouseCursor = false;
			}
		} else
		{
			
			WidgetToToggle->SetVisibility(ESlateVisibility::Visible);
			FInputModeGameAndUI InputMode;
			InputMode.SetWidgetToFocus(WidgetToToggle->TakeWidget());
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PC->SetInputMode(InputMode);
			PC->SetShowMouseCursor(true);
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

UQuickBarUserWidget* AWitchPTHUD::GetQuickBarUserWidget() const
{
	if (QuickBarUserWidget)
	{
		if (UQuickBarUserWidget* QuickBarWidget = Cast<UQuickBarUserWidget>(QuickBarUserWidget))
		{
			return QuickBarWidget;
		}
	}
	return nullptr;
}

UWitchPTUserWidget* AWitchPTHUD::GetInventoryUserWidget() const
{
	if (InventoryWidget)
	{
		if (UInventoryUserWidget* InventoryWidgetInstance = Cast<UInventoryUserWidget>(InventoryWidget))
		{
			return InventoryWidgetInstance;
		}
	}
	return nullptr;
}



