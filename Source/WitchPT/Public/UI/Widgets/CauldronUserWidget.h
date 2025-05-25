// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WitchPTUserWidget.h"
#include "CauldronUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class WITCHPT_API UCauldronUserWidget : public UWitchPTUserWidget
{
	GENERATED_BODY()
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
};
