// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WitchPTUserWidget.h"
#include "GameplayTagContainer.h"
#include "Subsystems/UIManagerSubsystem.h"
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
	virtual void NativeConstruct() override;

	// UPROPERTY(EditDefaultsOnly, Category = "Layout")
	// TMap<FGameplayTag, FUIActivationContext> InitialScreens;

	// ------------------------------- Main functions ------------------------------- //
	UFUNCTION(BlueprintCallable, Category = "UI")
	bool RegisterLayer(FGameplayTag LayerTag, UWitchPTUILayer* InLayer);
	
	UFUNCTION(BlueprintCallable, Category = "UI")
	bool UnRegisterLayer(FGameplayTag LayerTag);
	
	UFUNCTION(BlueprintCallable, Category = "UI")
	UUserWidget* PushContentToLayer(FGameplayTag LayerTag, TSubclassOf<UUserWidget> WidgetClass);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void PopContentFromLayer(FGameplayTag LayerTag);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ClearAllLayers();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ClearAllLayersExcept(FGameplayTag LayerTag);
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ClearLayer(FGameplayTag LayerTag);
	UFUNCTION(BlueprintCallable, Category = "UI")
	void PushInitialScreens();
	

private:
	
	UPROPERTY(VisibleAnywhere)
	TMap<FGameplayTag, TObjectPtr<UWitchPTUILayer>> Layers;
	
};


