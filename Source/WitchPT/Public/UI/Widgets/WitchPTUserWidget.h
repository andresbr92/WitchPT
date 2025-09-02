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
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

	UFUNCTION(BlueprintImplementableEvent)
	void OnWidgetControllerSet();
	UFUNCTION(BlueprintImplementableEvent)
	void OnModelViewSet();
	
	UPROPERTY(BlueprintReadOnly, Category="UI Pooling")
	TSubclassOf<UUserWidget> PoolKey;

	UPROPERTY(BlueprintReadOnly, Category="UI Pooling")
	TObjectPtr<UObject> LastContextObject;
};
