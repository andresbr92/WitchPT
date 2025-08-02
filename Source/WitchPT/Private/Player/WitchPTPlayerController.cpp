// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/WitchPTPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "AbilitySystem/WitchPTAbilitySystemComponent.h"
#include "Blueprint/UserWidget.h"
#include "Equipment/WitchPTQuickBarComponent.h"
#include "Input/WitchPTInputComponent.h"
#include "Inventory/WitchPTInventoryManagerComponent.h"
#include "Item/CauldronAltar.h"
#include "Item/RitualAltar.h"
#include "Item/Ritual/RitualFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Subsystems/UIManagerSubsystem.h"
#include "UI/HUD/WitchPTHUD.h"
#include "UI/WidgetControllers/CauldronWidgetController.h"
#include "UI/WidgetControllers/RitualWidgetController.h"
#include "UI/Widgets/CauldronUserWidget.h"
#include "UI/Widgets/Inventory/InventoryUserWidget.h"
#include "UI/Widgets/Inventory/RitualUserWidget.h"
#include "WitchPT/WitchPT.h"

AWitchPTPlayerController::AWitchPTPlayerController()
{
	bReplicates = true;
	InventoryManager = CreateDefaultSubobject<UWitchPTInventoryManagerComponent>("InventoryManager");
	InventoryManager->SetIsReplicated(true);
	QuickBarComponent = CreateDefaultSubobject<UWitchPTQuickBarComponent>("QuickBarComponent");
	QuickBarComponent->SetIsReplicated(true);
	
	
}

void AWitchPTPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}

void AWitchPTPlayerController::PreProcessInput(const float DeltaTime, const bool bGamePaused)
{
	Super::PreProcessInput(DeltaTime, bGamePaused);
}

void AWitchPTPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (WitchPtAbilitySystemComponent)
	{
		WitchPtAbilitySystemComponent->ProcessAbilityInput(DeltaTime, bGamePaused);
	}
	Super::PostProcessInput(DeltaTime, bGamePaused);
}

void AWitchPTPlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWitchPTPlayerController, InventoryManager);
}
void AWitchPTPlayerController::LocalToggleCauldronMenu()
{
	
	if (bCauldronMenuOpen)
	{
		CloseCauldronMenu();
	}
	else
	{
		OpenCauldronMenu();
	}
}

void AWitchPTPlayerController::LocalShowRitualWidget(ABaseInteractableAltar* Altar)
{
	if (!Altar || !IsLocalController())
	{
		UE_LOG(LogTemp, Error, TEXT("LocalShowRitualWidget: Invalid altar or not local controller!"));
		return;
	}
	
	if (Altar->IsA<ARitualAltar>())
	{
		// Cast the altar to a ritual altar
		ARitualAltar* RitualAltar = Cast<ARitualAltar>(Altar);
		
		// Get the HUD and show the ritual widget
		AWitchPTHUD* WitchPTHUD = Cast<AWitchPTHUD>(GetHUD());
		if (WitchPTHUD)
		{
			WitchPTHUD->ShowRitualWidget(RitualAltar);
			bRitualWidgetVisible = true;
			UE_LOG(LogTemp, Log, TEXT("LocalShowRitualWidget: Ritual widget shown for altar %s"), *RitualAltar->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("LocalShowRitualWidget: Failed to get WitchPTHUD!"));
		}
	}
}

void AWitchPTPlayerController::LocalHideRitualWidget()
{
	if (!IsLocalController())
	{
		UE_LOG(LogTemp, Error, TEXT("LocalHideRitualWidget: Not local controller!"));
		return;
	}
	
	// Get the HUD and hide the ritual widget
	AWitchPTHUD* WitchPTHUD = Cast<AWitchPTHUD>(GetHUD());
	if (WitchPTHUD)
	{
		WitchPTHUD->HideRitualWidget();
		bRitualWidgetVisible = false;
		UE_LOG(LogTemp, Log, TEXT("LocalHideRitualWidget: Ritual widget hidden"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("LocalHideRitualWidget: Failed to get WitchPTHUD!"));
	}
}

bool AWitchPTPlayerController::IsRitualWidgetVisible()
{
	return bRitualWidgetVisible;
}

void AWitchPTPlayerController::OpenCauldronMenu()
{
	AWitchPTHUD* WitchPTHUD = Cast<AWitchPTHUD>(GetHUD());
	if (WitchPTHUD)
	{
		// Find cauldron altar in level (or pass as parameter if available)
		ACauldronAltar* CauldronAltar = Cast<ACauldronAltar>(
			UGameplayStatics::GetActorOfClass(this, ACauldronAltar::StaticClass())
		);
        
		if (CauldronAltar)
		{
			WitchPTHUD->ShowCauldronWithInventory(CauldronAltar);
			bCauldronMenuOpen = true;
		}
	}
}

void AWitchPTPlayerController::CloseCauldronMenu()
{
	AWitchPTHUD* WitchPTHUD = Cast<AWitchPTHUD>(GetHUD());
	if (WitchPTHUD)
	{
		WitchPTHUD->HideCauldronWithInventory();
		bCauldronMenuOpen = false;
	}
}

void AWitchPTPlayerController::Client_ShowRitualWidget_Implementation(ABaseInteractableAltar* Altar)
{
	// Always call LocalShowRitualWidget - it handles the visibility check locally
	LocalShowRitualWidget(Altar);
}

void AWitchPTPlayerController::Client_HideRitualWidget_Implementation()
{
	// Always call LocalHideRitualWidget - it handles the visibility check locally
	LocalHideRitualWidget();
}



void AWitchPTPlayerController::Client_ToggleCauldronMenu_Implementation()
{
	LocalToggleCauldronMenu();
}
void AWitchPTPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(WitchPtiInputMappingContext)
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(WitchPtiInputMappingContext, 1);
	}
	CreateHUDWidget();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UUIManagerSubsystem* UIManagerSubsystem = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>())
		{
			UIManagerSubsystem->AddPlayer(LocalPlayer);
		}
	}
}

void AWitchPTPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UWitchPTInputComponent* WitchPtInputComponent = CastChecked<UWitchPTInputComponent>(InputComponent);
	WitchPtInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AWitchPTPlayerController::Move);
	WitchPtInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AWitchPTPlayerController::Look);
	WitchPtInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
	
}

void AWitchPTPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
	
}

void AWitchPTPlayerController::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		// add yaw and pitch input to controller
		ControlledPawn->AddControllerYawInput(LookAxisVector.X);
		ControlledPawn->AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AWitchPTPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (GetASC())
	{
		GetASC()->AbilityInputTagPressed(InputTag);
	}
}

void AWitchPTPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetASC())
	{
		GetASC()->AbilityInputTagReleased(InputTag);
	}
}

void AWitchPTPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (GetASC())
	{
		GetASC()->AbilityInputTagHeld(InputTag);
	}
}

void AWitchPTPlayerController::CreateHUDWidget()
{
	if (!IsLocalController()) return;
	HUDWidget = CreateWidget<UPointerWidget>(this, HUDWidgetClass);
	if (IsValid(HUDWidget))
	{
		HUDWidget->AddToViewport();
	}
}


UWitchPTAbilitySystemComponent* AWitchPTPlayerController::GetASC()
{
	if (WitchPtAbilitySystemComponent == nullptr)
	{
		WitchPtAbilitySystemComponent = Cast<UWitchPTAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return WitchPtAbilitySystemComponent;
}




