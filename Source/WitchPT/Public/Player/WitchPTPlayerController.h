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

	void LocalInitializeRitualUserWidget(ABaseInteractableAltar* Altar);
	bool HasRitualWidgetInitialized(ABaseInteractableAltar* Altar);

	





	UFUNCTION(Client, Reliable)
	void Client_InitializeRitualUserWidget(ABaseInteractableAltar* Altar);

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
	UFUNCTION(BlueprintCallable, Category= "Inventory")
	void ToggleCauldronMenu();
	void OpenCauldronMenu();
	void CloseCauldronMenu();
	bool bCauldronMenuOpen = false;
	
	// Array de altares para los que ya se ha creado un widget
	UPROPERTY()
	TObjectPtr<ARitualAltar> ThisRitualAltarHasWidget;
	UPROPERTY()
	TObjectPtr<UCauldronUserWidget> CauldronAltarMenu;
	
};

