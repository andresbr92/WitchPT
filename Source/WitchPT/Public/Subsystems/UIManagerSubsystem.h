// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "UI/Widgets/WitchPTUserWidget.h"
#include "UIManagerSubsystem.generated.h"

/**
 * 
 */

class UWitchPTUILayer;
struct FGameplayTag;
class UWitchPTPrimaryLayout;
class AWitchPTHUD;

UCLASS()
class WITCHPT_API UUIManagerSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "UI Manager")
	AWitchPTHUD* GetWitchPTHUD();
	
	UFUNCTION(BlueprintCallable, Category = "UI Manager")
	bool RegisterLayout(FGameplayTag LayoutTag, UWitchPTUILayer* InLayer);

	UFUNCTION(BlueprintCallable, Category = "UI Manager")
	bool UnRegisterLayout(FGameplayTag LayerTag);

	UFUNCTION(BlueprintCallable, Category = "UI Manager")
	UUserWidget* PushContentToLayer(FGameplayTag LayerTag, TSoftClassPtr<UUserWidget> WidgetClass);

	UFUNCTION(BlueprintCallable, Category = "UI Manager")
	void PopContentFromLayer(FGameplayTag LayerTag);

	UFUNCTION(BlueprintCallable, Category = "UI Manager")
	void ClearAllLayers();

	UFUNCTION(BlueprintCallable, Category = "UI Manager")
	void ClearLayerExcept(FGameplayTag LayerTag);

	UFUNCTION(BlueprintCallable, Category = "UI Manager")
	void ClearLayer(FGameplayTag LayerTag);

	UFUNCTION(BlueprintCallable, Category = "UI Manager")
	void FocusGame();

	UFUNCTION(BlueprintCallable, Category = "UI Manager")
	void FocusModal(UUserWidget* WidgetToFocus, bool bShowCursor = true, bool bUIOnlyInput = false);
private:
	
	UWitchPTUserWidget* GetPrimaryLayout();
	
};
