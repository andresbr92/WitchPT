// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "WitchPTHUD.generated.h"

class UQuickBarWidgetController;
class UCauldronWidgetController;
class UWitchPTWidgetController;
class URitualWidgetController;
class UAbilitySystemComponent;
class UAttributeSet;
class UWitchPTUserWidget;
class UInventoryWidgetController;
struct FWidgetControllerParams;
class UOverlayWidgetController;
/**
 * 
 */
UCLASS()
class WITCHPT_API AWitchPTHUD : public AHUD
{
	GENERATED_BODY()
public:
	UOverlayWidgetController* SetOverlayWidgetController(const FWidgetControllerParams& WCParams);
	UInventoryWidgetController* SetInventoryWidgetController(const FWidgetControllerParams& WCParams);
	URitualWidgetController* SetRitualWidgetController(const FWidgetControllerParams& WCParams);
	UCauldronWidgetController* SetCauldronWidgetController(const FWidgetControllerParams& WCParams);
	UQuickBarWidgetController* SetQuickBarWidgetController(const FWidgetControllerParams& WCParams);

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);
	void InitRitualWidget(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);
	void InitInventoryWidget(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);
	void InitCauldronWidget(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);
	void InitQuickBarWidget(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

	void ShowRitualWidget(class ARitualAltar* RitualAltar);
	void HideRitualWidget();
	void ShowInventoryWidget();
	void HideInventoryWidget();
	void ShowCauldronWithInventory(class ACauldronAltar* CauldronAltar);
	void HideCauldronWithInventory();



private:
	// ----------------------- User Widgets -----------------------
	UPROPERTY()
	TObjectPtr<UWitchPTUserWidget> QuickBarUserWidget;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UWitchPTUserWidget> QuickBarUserWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UWitchPTUserWidget> OverlayWidget;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UWitchPTUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UWitchPTUserWidget> InventoryWidget;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UWitchPTUserWidget> InventoryWidgetClass;

	UPROPERTY()
	TObjectPtr<UWitchPTUserWidget> CauldronWidget;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UWitchPTUserWidget> CauldronWidgetClass;
	bool bCauldronWidgetVisible = false;

	UPROPERTY()
	TObjectPtr<UWitchPTUserWidget> RitualWidget;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UWitchPTUserWidget> RitualWidgetClass;


	// ----------------------- Widget Controllers -----------------------
	UPROPERTY()
	TObjectPtr<UQuickBarWidgetController> QuickBarWidgetController;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UQuickBarWidgetController> QuickBarWidgetControllerClass;
	
	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UInventoryWidgetController> InventoryWidgetController;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UInventoryWidgetController> InventoryWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<URitualWidgetController> RitualWidgetController;
	UPROPERTY(EditAnywhere)
	TSubclassOf<URitualWidgetController> RitualWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UCauldronWidgetController> CauldronWidgetController;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UCauldronWidgetController> CauldronWidgetControllerClass;
};
