// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "WitchPTHUD.generated.h"

class UWitchPT_MVVMViewModelBase;
class UMVVM_OverlayViewModel;
class UQuickBarUserWidget;
class UWitchPTPrimaryLayout;
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
	virtual void BeginPlay() override;
	
	void InitializeViewModels(APlayerController *PC, APlayerState *PS, UAbilitySystemComponent *ASC, UAttributeSet *AS);

private:
	
	UPROPERTY(EditAnywhere)
	TMap<TSubclassOf<UWitchPT_MVVMViewModelBase>, FName> ViewModelClasses;
};
