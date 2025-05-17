// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/WitchPTPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "AbilitySystem/WitchPTAbilitySystemComponent.h"
#include "Blueprint/UserWidget.h"
#include "Input/WitchPTInputComponent.h"
#include "Inventory/WitchPTInventoryManagerComponent.h"
#include "Item/RitualAltar.h"
#include "Item/Ritual/RitualFunctionLibrary.h"
#include "Net/UnrealNetwork.h"
#include "UI/HUD/WitchPTHUD.h"
#include "UI/WidgetControllers/RitualWidgetController.h"
#include "UI/Widgets/Inventory/RitualUserWidget.h"
#include "WitchPT/WitchPT.h"

AWitchPTPlayerController::AWitchPTPlayerController()
{
	bReplicates = true;
	InventoryManager = CreateDefaultSubobject<UWitchPTInventoryManagerComponent>("InventoryManager");
	InventoryManager->SetIsReplicated(true);
	
	
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

void AWitchPTPlayerController::InitializeRitualUserWidget(ARitualAltar* RitualAltar)
{
	if (!RitualAltar || !RitualAltar->RitualUserWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("InitializeRitualUserWidget: Invalid altar or widget class!"));
		return;
	}
	
	URitualUserWidget* RitualUserWidget = CreateWidget<URitualUserWidget>(this, RitualAltar->RitualUserWidgetClass);
	
	if (IsValid(RitualUserWidget))
	{
		// Get or create the ritual widget controller
		URitualWidgetController* RitualWidgetController = URitualFunctionLibrary::SetRitualWidgetController(this);
		
		if (!RitualWidgetController)
		{
			UE_LOG(LogTemp, Error, TEXT("InitializeRitualUserWidget: Failed to get or create RitualWidgetController!"));
			return;
		}
		
		// Set widget controller reference for the user widget
		RitualUserWidget->SetWidgetController(RitualWidgetController);
		
		// Set ritual altar in the controller
		RitualWidgetController->SetRitualAltar(RitualAltar);
		
		// Add the widget to viewport
		RitualUserWidget->AddToViewport();
	}
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
