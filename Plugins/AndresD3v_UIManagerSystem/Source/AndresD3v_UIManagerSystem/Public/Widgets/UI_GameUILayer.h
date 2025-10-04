// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/UserWidgetPool.h"
#include "UI_GameUILayer.generated.h"

class UBorder;
/**
 * 
 */
UCLASS()
class ANDRESD3V_UIMANAGERSYSTEM_API UUI_GameUILayer : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
	UFUNCTION(Blueprintable, Category= "UI Layer")
	UUserWidget* PushContent(TSubclassOf<UUserWidget> WidgetClass);

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	
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

	void ShowTop();
	void CollapseTop();

	FUserWidgetPool WidgetPool;
};
