// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/WitchPTUserWidget.h"
#include "UWitchPTUILayer.generated.h"

class UBorder;
/**
 * 
 */
UCLASS()
class WITCHPT_API UUWitchPTUILayer : public UWitchPTUserWidget
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(Blueprintable, Category= "UI Layer")
	UUserWidget* PushContent(TSoftClassPtr<UUserWidget> WidgetClass);
	
	UFUNCTION(BlueprintCallable, Category= "UI Layer")
	void PopContent();

	UFUNCTION(BlueprintPure, Category= "UI Layer")
	UUserWidget* GetTopScreen() const;
	UFUNCTION(BlueprintCallable, Category= "UI Layer")
	FText GetStackListNames();
	UFUNCTION(BlueprintCallable, Category= "UI Layer")
	void ClearStack();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Border;
private:
	UPROPERTY()
	TArray<TObjectPtr<UUserWidget>> Stack;
	UPROPERTY()
	TObjectPtr<UUserWidget> TopWidget = nullptr;
	UPROPERTY()
	TObjectPtr<UUserWidget> PushedWidget = nullptr;

	// HELPER FUNCTIONS
	void CollapseTop();
	void ShowTop();
	
	
};
