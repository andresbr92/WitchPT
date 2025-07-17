// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/UIManagerSubsystem.h"

#include "GameplayTagContainer.h"
#include "Item/CauldronAltar.h"
#include "Item/RitualAltar.h"
#include "Player/WitchPTPlayerState.h"
#include "UI/HUD/WitchPTHUD.h"
#include "UI/WidgetControllers/CauldronWidgetController.h"
#include "UI/WidgetControllers/GenericContainerWidgetController.h"
#include "UI/WidgetControllers/RitualWidgetController.h"
#include "UI/WidgetControllers/WitchPTWidgetController.h"
#include "UI/Widgets/WitchPTPrimaryLayout.h"
#include "UI/Widgets/WitchPTUserWidget.h"

AWitchPTHUD* UUIManagerSubsystem::GetWitchPTHUD()
{
	if (ULocalPlayer * LocalPlayer = GetLocalPlayer())
	{
		if (APlayerController* PlayerController = LocalPlayer->GetPlayerController(GetWorld()))
		{
			if (AWitchPTHUD* WitchPTHUD = Cast<AWitchPTHUD>(PlayerController->GetHUD()))
			{
				return WitchPTHUD;
			}
		}
	}
	return nullptr;
}

bool UUIManagerSubsystem::RegisterLayout(FGameplayTag LayoutTag, UWitchPTUILayer* InLayer)
{
	if (LayoutTag.IsValid() && InLayer != nullptr)
	{
		if (AWitchPTHUD* WitchPTHUD = GetWitchPTHUD())
		{
			if (UWitchPTPrimaryLayout* PrimaryLayout = WitchPTHUD->GetPrimaryLayout())
			{
				return PrimaryLayout->RegisterLayer(LayoutTag, InLayer);
			}
		}
	}
	return false;
}

bool UUIManagerSubsystem::UnRegisterLayout(FGameplayTag LayerTag)
{
	if (LayerTag.IsValid())
	{
		if (AWitchPTHUD* WitchPTHUD = GetWitchPTHUD())
		{
			if (UWitchPTPrimaryLayout* PrimaryLayout = WitchPTHUD->GetPrimaryLayout())
			{
				return PrimaryLayout->UnRegisterLayer(LayerTag);
			}
		}
		
	}
	return false;
}

UUserWidget* UUIManagerSubsystem::PushContentToLayer(FGameplayTag LayerTag, FUIActivationContext ActivationContext)
{
	if (LayerTag.IsValid() && ActivationContext.WidgetClass.IsValid())
	{
		AWitchPTHUD* WitchPTHUD = GetWitchPTHUD();
		APlayerController* PC = GetLocalPlayer()->GetPlayerController(GetWorld());
	
		if (!WitchPTHUD || !PC) return nullptr;
	
		AWitchPTPlayerState* PS = PC->GetPlayerState<AWitchPTPlayerState>();
		UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
		UAttributeSet* AS = PS->GetAttributeSet();

		const FWidgetControllerParams WCParams(PC, PS, ASC, AS);
		TMap<TSubclassOf<UWitchPTWidgetController>, UWitchPTWidgetController*> ControllerPackage =
			WitchPTHUD->CreateWidgetsControllers(ActivationContext.RequiredControllers, WCParams, ActivationContext.ContextObject);
		if (ControllerPackage.Num() == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("UIManager: No controllers created."));
		}
		UGenericContainerWidgetController* ContainerController = NewObject<UGenericContainerWidgetController>();
		ContainerController->SetControllerPackage(ControllerPackage);
		
		if (UWitchPTPrimaryLayout* PrimaryLayout = WitchPTHUD->GetPrimaryLayout())
		{
			UUserWidget* PushedWidget = PrimaryLayout->PushContentToLayer(LayerTag, ActivationContext.WidgetClass);
			if (PushedWidget)
			{
				UWitchPTUserWidget* WitchPtUserWidget = Cast<UWitchPTUserWidget>(PushedWidget);
				WitchPtUserWidget->SetWidgetController(ContainerController);
			}
			return PushedWidget;
			
		}
		
		
	}
	return nullptr;
}

void UUIManagerSubsystem::PopContentFromLayer(FGameplayTag LayerTag)
{
	if (LayerTag.IsValid())
	{
		if (AWitchPTHUD* WitchPTHUD = GetWitchPTHUD())
		{
			if (UWitchPTPrimaryLayout* PrimaryLayout = WitchPTHUD->GetPrimaryLayout())
			{
				PrimaryLayout->PopContentFromLayer(LayerTag);
				UE_LOG(LogTemp, Log, TEXT("Popping content from layer: %s"), *LayerTag.ToString());
			}
		}
	}
}

void UUIManagerSubsystem::ClearAllLayers()
{
}

void UUIManagerSubsystem::ClearLayerExcept(FGameplayTag LayerTag)
{
}

void UUIManagerSubsystem::ClearLayer(FGameplayTag LayerTag)
{
}

UWitchPTUserWidget* UUIManagerSubsystem::GetPrimaryLayout()
{
	if (AWitchPTHUD* WitchPTHUD = GetWitchPTHUD())
	{
		if (WitchPTHUD->GetPrimaryLayout())
		{
			return WitchPTHUD->GetPrimaryLayout();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Primary Layout is not initialized in WitchPTHUD!"));
		}
	}
	return nullptr;
}



void UUIManagerSubsystem::FocusGame()
{
	// get player controller
	if (APlayerController* PlayerController = GetLocalPlayer()->GetPlayerController(GetWorld()))
	{
		// set input mode to game only
		PlayerController->SetInputMode(FInputModeGameOnly());
		// hide mouse cursor
		PlayerController->bShowMouseCursor = false;
	}
}

void UUIManagerSubsystem::FocusModal(UUserWidget* WidgetToFocus, bool bShowCursor, bool bUIOnlyInput)
{
	if (APlayerController* PlayerController = GetLocalPlayer()->GetPlayerController(GetWorld()))
	{
		// Check if the widget is valid before trying to focus it
		if (!IsValid(WidgetToFocus))
		{
			UE_LOG(LogTemp, Warning, TEXT("FocusModal: WidgetToFocus is invalid or null"));
			return;
		}

		if(bUIOnlyInput)
		{
			FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(WidgetToFocus->TakeWidget());
			PlayerController->SetInputMode(InputMode);
			bShowCursor ? PlayerController->bShowMouseCursor = true : PlayerController->bShowMouseCursor = false;
		}
		else
		{
			FInputModeGameAndUI InputMode;
			InputMode.SetWidgetToFocus(WidgetToFocus->TakeWidget());
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			bShowCursor ? PlayerController->bShowMouseCursor = true : PlayerController->bShowMouseCursor = false;
			PlayerController->SetInputMode(InputMode);
		}
	}
}
