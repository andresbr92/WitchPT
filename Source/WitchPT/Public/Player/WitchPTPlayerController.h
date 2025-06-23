// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UI/Widgets/PointerWidget.h"
#include "WitchPTPlayerController.generated.h"

class UCauldronUserWidget;
class ABaseInteractableAltar;
class ACauldronAltar;
class ARitualAltar;
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

	void LocalShowRitualWidget(ABaseInteractableAltar* Altar);
	void LocalHideRitualWidget();
	bool IsRitualWidgetVisible();
	void LocalToggleCauldronMenu();
	



	UFUNCTION(Client, Reliable)
	void Client_ShowRitualWidget(ABaseInteractableAltar* Altar);

	UFUNCTION(Client, Reliable)
	void Client_HideRitualWidget();

	UFUNCTION(Client, Reliable)
	void Client_ToggleCauldronMenu();

	UFUNCTION(BlueprintCallable, Category= "Inventory")
	void OpenInventoryMenu();
	UFUNCTION(BlueprintCallable, Category= "Inventory")
	void CloseInventoryMenu();

	// Debug console commands for testing (Step 4 testing)
	UFUNCTION(Exec, Category = "Debug")
	void ShowRitualWidgetDebug();
	UFUNCTION(Exec, Category = "Debug") 
	void HideRitualWidgetDebug();
	UFUNCTION(Exec, Category = "Debug")
	void ToggleRitualWidgetDebug();


	UPROPERTY(Replicated, VisibleAnywhere)
	TObjectPtr<UWitchPTInventoryManagerComponent> InventoryManager;
	UPROPERTY()
	TObjectPtr<UPointerWidget> HUDWidget;
	UWitchPTInventoryManagerComponent* GetInventoryManager() { return InventoryManager; }
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
	
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<UPointerWidget> HUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<UCauldronUserWidget> CauldronAltarWidgetClass;

	

private:

	void Move(const FInputActionValue& InputActionValue);

	void Look(const FInputActionValue& Value);

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	void ConstructCauldronWidget();

	void CreateHUDWidget();
	void OpenCauldronMenu();
	void CloseCauldronMenu();
	bool bCauldronMenuOpen = false;
	
	// Track ritual widget visibility state
	bool bRitualWidgetVisible = false;
	
	UPROPERTY()
	TObjectPtr<UCauldronUserWidget> CauldronAltarMenu;
	
};

