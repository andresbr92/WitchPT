// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WitchPTWidgetController.h"
#include "InventoryWidgetController.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemAddedSignature, UWitchPTInventoryItemInstance*, ItemAdded);
UCLASS(BlueprintType, Blueprintable)
class WITCHPT_API UInventoryWidgetController : public UWitchPTWidgetController
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	virtual void BindCallbacksToDependencies() override;


	UPROPERTY(BlueprintAssignable, Category="Inventory")
	FOnItemAddedSignature OnItemAddedDelegate;
	UPROPERTY(BlueprintAssignable, Category="Inventory")
	FOnItemAddedSignature OnItemStackChangedDelegate;
};
