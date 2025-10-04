// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "UI_GameUILayout.generated.h"

class UUI_GameUILayer;
/**
 * 
 */
UCLASS()
class ANDRESD3V_UIMANAGERSYSTEM_API UUI_GameUILayout : public UUserWidget
{
	GENERATED_BODY()
public:
	UUI_GameUILayout(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;

	// ------------------------------- Main functions ------------------------------- //
	UFUNCTION(BlueprintCallable, Category = "AndresD3v|UI Manager System")
	bool RegisterLayer(FGameplayTag LayerTag, UUI_GameUILayer* InLayer);
	
	UFUNCTION(BlueprintCallable, Category = "AndresD3v|UI Manager System")
	bool UnRegisterLayer(FGameplayTag LayerTag);
	
	UFUNCTION(BlueprintCallable, Category = "AndresD3v|UI Manager System")
	UUserWidget* PushContentToLayer(FGameplayTag LayerTag, TSubclassOf<UUserWidget> WidgetClass);

	UFUNCTION(BlueprintCallable, Category = "AndresD3v|UI Manager System")
	void PopContentFromLayer(FGameplayTag LayerTag);

	UFUNCTION(BlueprintCallable, Category = "AndresD3v|UI Manager System")
	void ClearAllLayers();
	UFUNCTION(BlueprintCallable, Category = "AndresD3v|UI Manager System")
	void ClearAllLayersExcept(FGameplayTag LayerTag);
	UFUNCTION(BlueprintCallable, Category = "AndresD3v|UI Manager System")
	void ClearLayer(FGameplayTag LayerTag);
	

private:
	
	UPROPERTY(VisibleAnywhere)
	TMap<FGameplayTag, TObjectPtr<UUI_GameUILayer>> Layers;
};
