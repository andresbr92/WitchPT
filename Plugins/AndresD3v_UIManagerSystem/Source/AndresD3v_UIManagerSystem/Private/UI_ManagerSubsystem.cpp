// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_ManagerSubsystem.h"

#include "UI_GameUIContext.h"
#include "UI_GameUIPolicy.h"
#include "UI_SystemSettings.h"
#include "Widgets/UI_GameUILayout.h"

void UUI_ManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	if (UUI_SystemSettings::Get()->GameUIPolicyClass.IsNull())
	{
		UE_LOG(LogTemp, Warning, TEXT("No GameUIPolicyClass set in GenericUISystemSettings!"));
		return;
	}
	if (!CurrentPolicy)
	{
		if (TSubclassOf<UUI_GameUIPolicy> PolicyClass = UUI_SystemSettings::Get()->GameUIPolicyClass.LoadSynchronous())
		{
			if (UUI_GameUIPolicy* NewPolicy = NewObject<UUI_GameUIPolicy>(this, PolicyClass))
			{
				CurrentPolicy = NewPolicy;
				UE_LOG(LogTemp, Log, TEXT("UIManagerSubsystem initialized with policy: %s"), *CurrentPolicy->GetName());
			}
			
		}
	}
}

void UUI_ManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();
	CurrentPolicy = nullptr;
	UE_LOG(LogTemp, Log, TEXT("UIManagerSubsystem deinitialized."));
}




// ------------------------------------------------------------------
// ------------------------- MAIN FUNCTIONS -------------------------
// ------------------------------------------------------------------
UUserWidget* UUI_ManagerSubsystem::PushContentToLayer_ForPlayer(const APlayerController* PlayerController, UPARAM(meta = (Categories = "UI.Layer")) FGameplayTag LayerTag, UPARAM(meta = (AllowAbstract = false)) TSubclassOf<UUserWidget> WidgetClass)
{
	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(PlayerController->GetLocalPlayer()))
	{
		if (UUI_GameUILayout* RootLayout = CurrentPolicy->GetRootLayout(LocalPlayer))
		{
			
			return RootLayout->PushContentToLayer(LayerTag, WidgetClass);
			
		}
	}
	return nullptr;

}

void UUI_ManagerSubsystem::PushTemporalContentToLayer_ForPlayer(const APlayerController* PlayerController,
	FGameplayTag LayerTag, TSubclassOf<UUserWidget> WidgetClass, float Duration)
{
	PushContentToLayer_ForPlayer(PlayerController, LayerTag, WidgetClass);
	
	if (PlayerController) 
	{
		FTimerHandle TimerHandle;
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUFunction(this, FName("PopContentFromLayer_ForPlayer"), PlayerController, LayerTag, 1);
		
		PlayerController->GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, Duration, false);
	}
	
}

void UUI_ManagerSubsystem::PopContentFromLayer_ForPlayer(const APlayerController* PlayerController, FGameplayTag LayerTag, int32 RemainNum)
{
	if (LayerTag.IsValid() && ensure(PlayerController))
	{
		if (CurrentPolicy)
		{
			if (UUI_GameUILayout* RootLayout = CurrentPolicy->GetRootLayout(PlayerController->GetLocalPlayer()))
			{
				// TODO: Implement RemainNum logic if needed
				RootLayout->PopContentFromLayer(LayerTag);
			}
		}
	}
}
void UUI_ManagerSubsystem::FocusGame(APlayerController* PlayerController)
{
	if (PlayerController)
	{
		PlayerController->SetInputMode(FInputModeGameOnly());
		PlayerController->bShowMouseCursor = false;
	}
}

void UUI_ManagerSubsystem::FocusModal(APlayerController* PlayerController, UUserWidget* WidgetToFocus, bool bShowCursor, bool bUIOnlyInput)
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
// ------------------------------------------------------------------
// ------------------------- PLAYER FUNCTIONS -----------------------
// ------------------------------------------------------------------

void UUI_ManagerSubsystem::AddPlayer(ULocalPlayer* LocalPlayer)
{
	NotifyPlayerAdded(LocalPlayer);
}

void UUI_ManagerSubsystem::RemovePlayer(ULocalPlayer* LocalPlayer)
{
	NotifyPlayerRemoved(LocalPlayer);
}
// ------------------------------------------------------------------
// --------------------------- GAME UI CONTEXT FUNCTIONS ------------
// ------------------------------------------------------------------
void UUI_ManagerSubsystem::RegisterUIContext_ForPlayer(APlayerController* PlayerController, UUI_GameUIContext* Context)
{
	if (PlayerController && CurrentPolicy && Context)
	{
		if (const ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(PlayerController->GetLocalPlayer()))
		{
			CurrentPolicy->AddContext(LocalPlayer, Context);
		}

	}
}

void UUI_ManagerSubsystem::UnRegisterUIContext_ForPlayer(APlayerController* PlayerController,
	TSubclassOf<UUI_GameUIContext> ContextClass)
{
	if (PlayerController && CurrentPolicy && ContextClass)
	{
		if (const ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(PlayerController->GetLocalPlayer()))
		{
			CurrentPolicy->RemoveContext(LocalPlayer, ContextClass);
		}
		
	}
}

void UUI_ManagerSubsystem::RegisterUIContext_ForActor(AActor* Actor, UUI_GameUIContext* Context)
{
	// TODO: Implement this function
}

bool UUI_ManagerSubsystem::FindUIContext_ForPlayer(ULocalPlayer* LocalPlayer,
	TSubclassOf<UUI_GameUIContext> ContextClass, UUI_GameUIContext*& OutContext)
{
	if (LocalPlayer && CurrentPolicy && ContextClass != nullptr)
	{
		if (UUI_GameUIContext* Context = CurrentPolicy->GetContext(LocalPlayer, ContextClass))
		{
			if (Context->GetClass() == ContextClass)
			{
				OutContext = Context;
				return true;
			}
		}
	}
	return false;
}

void UUI_ManagerSubsystem::NotifyPlayerAdded(ULocalPlayer* LocalPlayer)
{
	if (ensure(LocalPlayer) && CurrentPolicy)
	{
		CurrentPolicy->NotifyPlayerAdded(LocalPlayer);
	}
}

void UUI_ManagerSubsystem::NotifyPlayerRemoved(ULocalPlayer* LocalPlayer)
{
	if (ensure(LocalPlayer) && CurrentPolicy)
	{
		CurrentPolicy->NotifyPlayerRemoved(LocalPlayer);
	}
	
}

void UUI_ManagerSubsystem::NotifyPlayerDestroyed(ULocalPlayer* LocalPlayer)
{
	if (LocalPlayer && CurrentPolicy)
	{
		// CurrentPolicy->NotifyPlayerDestroyed(LocalPlayer);
	}
}







void UUI_ManagerSubsystem::ClearAllLayers()
{
}

void UUI_ManagerSubsystem::ClearLayerExcept(FGameplayTag LayerTag)
{
}

void UUI_ManagerSubsystem::ClearLayer(FGameplayTag LayerTag)
{
}

UUserWidget* UUI_ManagerSubsystem::GetPrimaryLayout(const APlayerController* PlayerController) const
{
	if (UUI_GameUILayout* RootLayout = CurrentPolicy->GetRootLayout(PlayerController->GetLocalPlayer()))
	{
		return RootLayout;
	}
	return nullptr;
}
bool UUI_ManagerSubsystem::RegisterLayout(APlayerController* PlayerController, FGameplayTag LayoutTag, UUI_GameUILayer* InLayer)
{
	if (LayoutTag.IsValid() && InLayer != nullptr)
	{
	
		if (UUI_GameUILayout* RootLayout = CurrentPolicy->GetRootLayout(PlayerController->GetLocalPlayer()))
		{
			return RootLayout->RegisterLayer(LayoutTag, InLayer);
		}
		
	}
	return false;
}

bool UUI_ManagerSubsystem::UnRegisterLayout(APlayerController* PlayerController, FGameplayTag LayerTag)
{
	if (LayerTag.IsValid())
	{
	
		if (UUI_GameUILayout* RootLayout = CurrentPolicy->GetRootLayout(PlayerController->GetLocalPlayer()))
		{
			return RootLayout->UnRegisterLayer(LayerTag);
		}
		
	}
	return false;
}
