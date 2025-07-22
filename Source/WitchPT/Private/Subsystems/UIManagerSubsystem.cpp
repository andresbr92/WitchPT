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
		UUserWidget* FoundOrCreatedWidget = nullptr;
		TSubclassOf<UUserWidget> WidgetClass = ActivationContext.WidgetClass.LoadSynchronous();
		AWitchPTHUD* WitchPTHUD = GetWitchPTHUD();
		UWitchPTPrimaryLayout* PrimaryLayout = WitchPTHUD->GetPrimaryLayout();
		
		// ----------------------------------- FIND THE WIDGET ------------------------------------
		FWidgetPool* Pool = WidgetPools.Find(WidgetClass);
		if (Pool && Pool->AvailableWidgets.Num() > 0)
		{
			
			for (int32 i = Pool->AvailableWidgets.Num() - 1; i >= 0; --i)
			{
				UWitchPTUserWidget* PooledWidget = Cast<UWitchPTUserWidget>(Pool->AvailableWidgets[i]);
				if (PooledWidget)
				{
					
					if (PooledWidget->LastContextObject == ActivationContext.ContextObject)
					{
						// ¡Match perfecto! Lo reutilizamos.
						FoundOrCreatedWidget = PooledWidget;
						Pool->AvailableWidgets.RemoveAt(i); // Lo sacamos del pool
						
						PrimaryLayout->PushContentToLayer(LayerTag, FoundOrCreatedWidget);
						UE_LOG(LogTemp, Log, TEXT("Reusing widget for class:  %s"), *WidgetClass->GetName());
						return FoundOrCreatedWidget;
					}
				}
			}
		}
		// ----------------------------------- CREATE A NEW WIDGET ------------------------------------
		if (!FoundOrCreatedWidget)
		{
			
			
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
			FoundOrCreatedWidget = CreateWidget<UUserWidget>(PC, WidgetClass);
			
			if (PrimaryLayout)
			{
				PrimaryLayout->PushContentToLayer(LayerTag, FoundOrCreatedWidget);
				
				UWitchPTUserWidget* WitchPtUserWidget = Cast<UWitchPTUserWidget>(FoundOrCreatedWidget);
				WitchPtUserWidget->SetPoolingInfo(WidgetClass, ActivationContext.ContextObject);
				WitchPtUserWidget->SetWidgetController(ContainerController);
				
				return FoundOrCreatedWidget;
			}
		}
	}
	return nullptr;
}

void UUIManagerSubsystem::ReleaseWidgetToPool(UUserWidget* Widget)
{
	if (UWitchPTUserWidget* WitchWidget = Cast<UWitchPTUserWidget>(Widget))
	{
		TSubclassOf<UUserWidget> Key = WitchWidget->PoolKey;
		if (Key)
		{
			// Opcional: Lógica de destrucción si el contexto es "sucio"
			if (WitchWidget->LastContextObject != nullptr)
			{
				// Si decidimos que los widgets con contexto NUNCA se reutilizan si el contexto puede cambiar,
				// los destruimos aquí en lugar de devolverlos al pool.
				// Esto es una decisión de diseño, el enfoque híbrido es más óptimo.
				// Para este ejemplo, los devolvemos al pool.
			}

			FWidgetPool& Pool = WidgetPools.FindOrAdd(Key);
			Pool.AvailableWidgets.Add(WitchWidget);
			UE_LOG(LogTemp, Log, TEXT("Devolviendo widget al pool para la clase %s"), *Key->GetName());
			// Print all available widgets in the pool for debugging
			for (UUserWidget* AvailableWidget : Pool.AvailableWidgets)
			{
				if (AvailableWidget)
				{
					UE_LOG(LogTemp, Log, TEXT("Available widget: %s"), *AvailableWidget->GetName());
				}
			}
		}
	}
}

void UUIManagerSubsystem::ClearAllPools()
{
	for (auto& Pair : WidgetPools)
	{
		for (UUserWidget* Widget : Pair.Value.AvailableWidgets)
		{
			if (Widget)
			{
				Widget->RemoveFromParent();
				Widget->MarkAsGarbage();
			}
		}
	}
	WidgetPools.Empty();
	UE_LOG(LogTemp, Log, TEXT("All widget pools have been cleaned."));
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
