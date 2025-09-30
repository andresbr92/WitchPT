// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tasks/TargetingFilterTask_BasicFilterTemplate.h"
#include "Int_TargetingFilterTask_InteractionSmartObjects.generated.h"

/**
 * 
 */
UCLASS()
class ANDRESD3V_INTERACTIONSYSTEM_API
	UInt_TargetingFilterTask_InteractionSmartObjects : public UTargetingFilterTask_BasicFilterTemplate
{
	GENERATED_BODY()
protected:
	/**
	 * Determines if a target should be filtered based on interaction criteria.
	 * @param TargetingHandle The targeting request handle.
	 * @param TargetData The target data.
	 * @return True if the target should be filtered, false otherwise.
	 */
	virtual bool ShouldFilterTarget(const FTargetingRequestHandle& TargetingHandle, const FTargetingDefaultResultData& TargetData) const override;
};
