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
	 if (!LayerTag.IsValid() || ActivationContext.WidgetClass.IsNull())
    {
        return nullptr;
    }

    TSubclassOf<UUserWidget> WidgetClass = ActivationContext.WidgetClass.LoadSynchronous();
    if (!WidgetClass) return nullptr;

    UWitchPTUserWidget* TargetWidget = nullptr;

    // --- FASE 1: ADQUISICIÓN DEL WIDGET (POOL O NUEVO) ---
    FWidgetPool* Pool = WidgetPools.Find(WidgetClass);
    if (Pool && Pool->AvailableWidgets.Num() > 0)
    {
        // Busca un widget compatible
        for (int32 i = Pool->AvailableWidgets.Num() - 1; i >= 0; --i)
        {
            UWitchPTUserWidget* PooledWidget = Cast<UWitchPTUserWidget>(Pool->AvailableWidgets[i]);
            if (PooledWidget && PooledWidget->LastContextObject == ActivationContext.ContextObject)
            {
                TargetWidget = PooledWidget;
                Pool->AvailableWidgets.RemoveAt(i);
                UE_LOG(LogTemp, Log, TEXT("Reusing widget for class: %s"), *WidgetClass->GetName());
                break;
            }
        }
    }

    if (!TargetWidget)
    {
        // No se encontró uno compatible, o el pool está vacío. Creamos uno nuevo.
        TargetWidget = CreateWidget<UWitchPTUserWidget>(GetWorld(), WidgetClass);
        UE_LOG(LogTemp, Log, TEXT("Creating new widget for class: %s"), *WidgetClass->GetName());
    }

    if (!TargetWidget)
    {
        return nullptr;
    }

    // --- FASE 2: CREACIÓN Y CABLEADO DE CONTROLADORES ---
    AWitchPTHUD* WitchPTHUD = GetWitchPTHUD();
    APlayerController* PC = GetLocalPlayer()->GetPlayerController(GetWorld());
    if (!WitchPTHUD || !PC) return nullptr;

    AWitchPTPlayerState* PS = PC->GetPlayerState<AWitchPTPlayerState>();
    const FWidgetControllerParams WCParams(PC, PS, PS->GetAbilitySystemComponent(), PS->GetAttributeSet());

    // La fábrica ahora solo crea y prepara los controladores, sin activarlos.
    TMap<TSubclassOf<UWitchPTWidgetController>, UWitchPTWidgetController*> ControllerPackage =
        WitchPTHUD->CreateWidgetsControllers(ActivationContext.RequiredControllers, WCParams, ActivationContext.ContextObject);

    // Creamos el contenedor
    UGenericContainerWidgetController* ContainerController = NewObject<UGenericContainerWidgetController>();
    ContainerController->SetControllerPackage(ControllerPackage);

    // Cableamos: Inyectamos el contenedor en la View. Esto dispara el OnWidgetControllerSet en BP.
    TargetWidget->SetPoolingInfo(WidgetClass, ActivationContext.ContextObject);
    TargetWidget->SetWidgetController(ContainerController);
    
    // --- FASE 3: ACTIVACIÓN DE CONTROLADORES ---
    // Ahora que la UI está completamente conectada, activamos los controladores.
    for (auto const& [Key, Val] : ControllerPackage)
    {
        if (Val)
        {
            Val->Activate();
        }
    }

    // --- FASE 4: MOSTRAR EN PANTALLA ---
    if (UWitchPTPrimaryLayout* PrimaryLayout = WitchPTHUD->GetPrimaryLayout())
    {
        PrimaryLayout->PushContentToLayer(LayerTag, TargetWidget);
    }
    
    return TargetWidget;
}

void UUIManagerSubsystem::ReleaseWidgetToPool(UUserWidget* Widget)
{
	 if (UWitchPTUserWidget* WitchWidget = Cast<UWitchPTUserWidget>(Widget))
    {
        // Desactivamos los controladores antes de guardarlo.
        if (UGenericContainerWidgetController* Container = Cast<UGenericContainerWidgetController>(WitchWidget->WidgetController))
        {
            // Opcional pero recomendado: un método para obtener el paquete
            TMap<TSubclassOf<UWitchPTWidgetController>, UWitchPTWidgetController*> ControllerPackage = Container->GetControllerPackage();
            for(auto const& [Key, Val] : ControllerPackage) { Val->Deactivate(); }
        }
        
        TSubclassOf<UUserWidget> Key = WitchWidget->PoolKey;
        if (Key)
        {
            FWidgetPool& Pool = WidgetPools.FindOrAdd(Key);
            Pool.AvailableWidgets.Add(WitchWidget);
            UE_LOG(LogTemp, Log, TEXT("Returning widget to pool for class %s"), *Key->GetName());
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
