// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "WitchPTHUD.generated.h"

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
	UWitchPTWidgetController* SetCauldronWidgetController(const FWidgetControllerParams& WCParams);

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);



private:
	UPROPERTY()
	TObjectPtr<UWitchPTUserWidget> OverlayWidget;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UWitchPTUserWidget> OverlayWidgetClass;


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
	TObjectPtr<UWitchPTWidgetController> CauldronWidgetController;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UWitchPTWidgetController> CauldronWidgetControllerClass;
};
