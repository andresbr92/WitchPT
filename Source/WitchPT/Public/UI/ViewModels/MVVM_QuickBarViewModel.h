// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WitchPT_MVVMViewModelBase.h"
#include "MVVM_QuickBarViewModel.generated.h"

/**
 * 
 */
UCLASS()
class WITCHPT_API UMVVM_QuickBarViewModel : public UWitchPT_MVVMViewModelBase
{
	GENERATED_BODY()
public:
	virtual void BindCallbacksToDependencies() override;
	virtual void BroadcastInitialValues() override;

private:
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess))
	int32 ActiveSlotIndex = 0;

public:
	void SetActiveSlotIndex(int32 NewActiveSlotIndex);
	int32 GetActiveSlotIndex() const { return ActiveSlotIndex; }

	
};
