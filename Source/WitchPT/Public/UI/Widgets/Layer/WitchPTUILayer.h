// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/WitchPTUserWidget.h"
#include "WitchPTUILayer.generated.h"

class UBorder;
/**
 * 
 */
UCLASS()
class WITCHPT_API UWitchPTUILayer : public UWitchPTUserWidget
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(Blueprintable, Category= "UI Layer")
	UUserWidget* PushContent(UUserWidget* InUserWidget);
	
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
	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<UUserWidget>> Stack;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UUserWidget> TopWidget = nullptr;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UUserWidget> PushedWidget = nullptr;

	void ShowTop();
	void CollapseTop();
	// HELPER FUNCTIONS

	
	
	
};
