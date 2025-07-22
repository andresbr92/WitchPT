// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WitchPTUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class WITCHPT_API UWitchPTUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

	UFUNCTION(BlueprintImplementableEvent)
	void OnWidgetControllerSet();

	void SetPoolingInfo(const TSubclassOf<UUserWidget>& InPoolKey, UObject* InContextObject);

	UPROPERTY(BlueprintReadOnly, Category="UI Pooling")
	TSubclassOf<UUserWidget> PoolKey;

	UPROPERTY(BlueprintReadOnly, Category="UI Pooling")
	TObjectPtr<UObject> LastContextObject;
};
