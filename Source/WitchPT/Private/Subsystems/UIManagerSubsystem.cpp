// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/UIManagerSubsystem.h"

#include "GameplayTagContainer.h"
#include "Item/CauldronAltar.h"
#include "Item/RitualAltar.h"
#include "Player/WitchPTPlayerState.h"
#include "UI/GenericUISystemSettings.h"
#include "UI/WitchPT_GameUIPolicy.h"
#include "UI/HUD/WitchPTHUD.h"
#include "UI/Widgets/WitchPTPrimaryLayout.h"
#include "UI/Widgets/WitchPTUserWidget.h"

void UUIManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	if (UGenericUISystemSettings::Get()->GameUIPolicyClass.IsNull())
	{
		UE_LOG(LogTemp, Warning, TEXT("No GameUIPolicyClass set in GenericUISystemSettings!"));
		return;
	}
	if (!CurrentPolicy)
	{
		if (TSubclassOf<UWitchPT_GameUIPolicy> PolicyClass = UGenericUISystemSettings::Get()->GameUIPolicyClass.LoadSynchronous())
		{
			if (UWitchPT_GameUIPolicy* NewPolicy = NewObject<UWitchPT_GameUIPolicy>(this, PolicyClass))
			{
				CurrentPolicy = NewPolicy;
				UE_LOG(LogTemp, Log, TEXT("UIManagerSubsystem initialized with policy: %s"), *CurrentPolicy->GetName());
			}
			
		}
	}
}

void UUIManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();
	CurrentPolicy = nullptr;
	UE_LOG(LogTemp, Log, TEXT("UIManagerSubsystem deinitialized."));
}

AWitchPTHUD* UUIManagerSubsystem::GetWitchPTHUD()
{
	// if (ULocalPlayer * LocalPlayer = GetLocalPlayer())
	// {
	// 	if (APlayerController* PlayerController = LocalPlayer->GetPlayerController(GetWorld()))
	// 	{
	// 		if (AWitchPTHUD* WitchPTHUD = Cast<AWitchPTHUD>(PlayerController->GetHUD()))
	// 		{
	// 			return WitchPTHUD;
	// 		}
	// 	}
	// }
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

UUserWidget* UUIManagerSubsystem::PushContentToLayer_ForPlayer(const APlayerController* PlayerController, UPARAM(meta = (Categories = "UI.Layer")) FGameplayTag LayerTag, UPARAM(meta = (AllowAbstract = false)) TSubclassOf<UUserWidget> WidgetClass)
{
	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(PlayerController->GetLocalPlayer()))
	{
		if (UWitchPTPrimaryLayout* RootLayout = CurrentPolicy->GetRootLayout(LocalPlayer))
		{
			
			return RootLayout->PushContentToLayer(LayerTag, WidgetClass);
			
		}
	}
	return nullptr;

}

void UUIManagerSubsystem::AddPlayer(ULocalPlayer* LocalPlayer)
{
	NotifyPlayerAdded(LocalPlayer);
}

void UUIManagerSubsystem::RemovePlayer(ULocalPlayer* LocalPlayer)
{
	NotifyPlayerRemoved(LocalPlayer);
}

void UUIManagerSubsystem::NotifyPlayerAdded(ULocalPlayer* LocalPlayer)
{
	if (ensure(LocalPlayer) && CurrentPolicy)
	{
		CurrentPolicy->NotifyPlayerAdded(LocalPlayer);
	}
}

void UUIManagerSubsystem::NotifyPlayerRemoved(ULocalPlayer* LocalPlayer)
{
	if (ensure(LocalPlayer) && CurrentPolicy)
	{
		CurrentPolicy->NotifyPlayerRemoved(LocalPlayer);
	}
	
}

void UUIManagerSubsystem::NotifyPlayerDestroyed(ULocalPlayer* LocalPlayer)
{
	if (LocalPlayer && CurrentPolicy)
	{
		// CurrentPolicy->NotifyPlayerDestroyed(LocalPlayer);
	}
}





void UUIManagerSubsystem::PopContentFromLayer_ForPlayer(const APlayerController* PlayerController, FGameplayTag LayerTag, int32 RemainNum)
{
	if (LayerTag.IsValid() && ensure(PlayerController))
	{
		if (CurrentPolicy)
		{
			if (UWitchPTPrimaryLayout* RootLayout = CurrentPolicy->GetRootLayout(PlayerController->GetLocalPlayer()))
			{
				// TODO: Implement RemainNum logic if needed
				RootLayout->PopContentFromLayer(LayerTag);
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
	// // get player controller
	// if (APlayerController* PlayerController = GetLocalPlayer()->GetPlayerController(GetWorld()))
	// {
	// 	// set input mode to game only
	// 	PlayerController->SetInputMode(FInputModeGameOnly());
	// 	// hide mouse cursor
	// 	PlayerController->bShowMouseCursor = false;
	// }
}

void UUIManagerSubsystem::FocusModal(APlayerController* PlayerController, UUserWidget* WidgetToFocus, bool bShowCursor, bool bUIOnlyInput)
{
	if (PlayerController)
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
