// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WitchPTIndicatorManagerComponent.generated.h"


class UIndicatorDescriptor;

UCLASS(BlueprintType, Blueprintable)
class WITCHPT_API UWitchPTIndicatorManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UWitchPTIndicatorManagerComponent(const FObjectInitializer& ObjectInitializer);

	static UWitchPTIndicatorManagerComponent* GetComponent(AController* Controller);

	UFUNCTION(BlueprintCallable, Category = Indicator)
	void AddIndicator(UIndicatorDescriptor* IndicatorDescriptor);
	
	UFUNCTION(BlueprintCallable, Category = Indicator)
	void RemoveIndicator(UIndicatorDescriptor* IndicatorDescriptor);

	DECLARE_EVENT_OneParam(ULyraIndicatorManagerComponent, FIndicatorEvent, UIndicatorDescriptor* Descriptor)
	FIndicatorEvent OnIndicatorAdded;
	FIndicatorEvent OnIndicatorRemoved;

	const TArray<UIndicatorDescriptor*>& GetIndicators() const { return Indicators; }

	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<UIndicatorDescriptor>> Indicators;
private:
};
