// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/UIManagerSubsystem.h"

#include "GameplayTagContainer.h"
#include "UI/HUD/WitchPTHUD.h"
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

UUserWidget* UUIManagerSubsystem::PushContentToLayer(FGameplayTag LayerTag, TSoftClassPtr<UUserWidget> WidgetClass)
{
	if (LayerTag.IsValid() && WidgetClass.IsValid())
	{
		if (AWitchPTHUD* WitchPTHUD = GetWitchPTHUD())
		{
			if (UWitchPTPrimaryLayout* PrimaryLayout = WitchPTHUD->GetPrimaryLayout())
			{
				return PrimaryLayout->PushContentToLayer(LayerTag, WidgetClass);
				
			}
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
void UUIManagerSubsystem::ShowFirstElementOfLayer(FGameplayTag LayerTag)
{
	if (AWitchPTHUD* WitchPTHUD = GetWitchPTHUD())
	{
		if (UWitchPTPrimaryLayout* PrimaryLayout = WitchPTHUD->GetPrimaryLayout())
		{
			PrimaryLayout->ShowFirstElementOfLayer(LayerTag);
		}
	}
}
void UUIManagerSubsystem::CollapseFirstElementOfLayer(FGameplayTag LayerTag)
{
	if (AWitchPTHUD* WitchPTHUD = GetWitchPTHUD())
	{
		if (UWitchPTPrimaryLayout* PrimaryLayout = WitchPTHUD->GetPrimaryLayout())
		{
			PrimaryLayout->CollapseFirstElementOfLayer(LayerTag);
		}
	}
}