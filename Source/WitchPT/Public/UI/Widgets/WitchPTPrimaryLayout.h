// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WitchPTUserWidget.h"
#include "GameplayTagContainer.h"
#include "WitchPTPrimaryLayout.generated.h"

class UWitchPTUILayer;
/**
 * 
 */
UCLASS()
class WITCHPT_API UWitchPTPrimaryLayout : public UWitchPTUserWidget
{
	GENERATED_BODY()
public:
	UWitchPTPrimaryLayout();

	UPROPERTY(EditDefaultsOnly, Category = "Layout")
	TMap<FGameplayTag, TSoftClassPtr<UWitchPTPrimaryLayout>> InitialScreens;

	// ------------------------------- Main functions ------------------------------- //
	UFUNCTION(BlueprintCallable, Category = "UI")
	bool RegisterLayer(FGameplayTag LayoutTag, UWitchPTUILayer* InLayer);
	UFUNCTION(BlueprintCallable, Category = "UI")
	bool UnRegisterLayer(FGameplayTag LayoutTag);

private:
	
	UPROPERTY(VisibleAnywhere)
	TMap<FGameplayTag, TObjectPtr<UWitchPTUILayer>> Layers;
	
	// Main layer, this displays the Health bar, Mana bar and the QuickBarWidget
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWitchPTUILayer> GameLayer;
	
	// This layer displays the inventory menu, the cauldron menu, etc
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWitchPTUILayer> GameMenuLayer;
	
	// This layer displays the main menu, the pause menu, etc
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWitchPTUILayer> MenuLayer;
	
	// This layer displays the modal windows, like the confirmation dialog, etc
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWitchPTUILayer> ModalLayer;
};
