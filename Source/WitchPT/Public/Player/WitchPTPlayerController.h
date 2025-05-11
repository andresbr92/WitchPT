// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Widgets/HUD/PointerWidget.h"
#include "WitchPTPlayerController.generated.h"

class UWitchPTIndicatorManagerComponent;
class UWitchPTInventoryManagerComponent;
struct FGameplayTag;
class UWitchPTAbilitySystemComponent;
class UWitchPTInputConfig;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class WITCHPT_API AWitchPTPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AWitchPTPlayerController();
	virtual void PlayerTick(float DeltaTime) override;
	virtual void PreProcessInput(const float DeltaTime, const bool bGamePaused) override;
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Replicated, VisibleAnywhere)
	TObjectPtr<UWitchPTInventoryManagerComponent> InventoryManager;
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	// Actions
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> WitchPtiInputMappingContext;
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, Category= "Input")
	TObjectPtr<UInputAction> LookAction;
	//InputConfig
	UPROPERTY(EditDefaultsOnly, Category= "Input")
	TObjectPtr<UWitchPTInputConfig> InputConfig;

	// Ability System Component
	UPROPERTY()
	TObjectPtr<UWitchPTAbilitySystemComponent> WitchPtAbilitySystemComponent;

	UWitchPTAbilitySystemComponent* GetASC();
	
private:

	void Move(const FInputActionValue& InputActionValue);

	void Look(const FInputActionValue& Value);

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	void CreateHUDWidget();
	
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<UPointerWidget> HUDWidgetClass;

	UPROPERTY()
	TObjectPtr<UPointerWidget> HUDWidget;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UWitchPTIndicatorManagerComponent> IndicatorManager;
	
};

