// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LayerWidget.generated.h"

class UCanvasPanel;
/**
 * 
 */
UCLASS()
class WITCHPT_API ULayerWidget : public UUserWidget
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidgetOptional))
	TObjectPtr<UCanvasPanel> IndicatorCanvas;
};
