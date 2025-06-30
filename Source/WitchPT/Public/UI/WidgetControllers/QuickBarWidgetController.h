// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WitchPTWidgetController.h"
#include "QuickBarWidgetController.generated.h"

class UWitchPTQuickBarComponent;
/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuickBarSlotChangedSignature, int32, NewActiveSlotIndex);
UCLASS(BlueprintType, Blueprintable)
class WITCHPT_API UQuickBarWidgetController : public UWitchPTWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	// Delegates for widgets to subscribe to
	UPROPERTY(BlueprintAssignable, Category = "QuickBar")
	FOnQuickBarSlotChangedSignature OnActiveSlotChangedDelegate;


protected:
	UFUNCTION()
	void OnActiveSlotChanged(int32 NewActiveSlotIndex);
	
	
	
};
