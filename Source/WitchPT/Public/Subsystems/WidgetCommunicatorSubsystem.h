// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "WidgetCommunicatorSubsystem.generated.h"

class UWitchPTInventoryItemInstance;

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemDraggedDelegate, UWitchPTInventoryItemInstance*, ItemInstance);
UCLASS(BlueprintType)
class WITCHPT_API UWidgetCommunicatorSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category= "WidgetCommunicator")
	void OnItemDragged(UWitchPTInventoryItemInstance* ItemInstance);

	
	UPROPERTY(BlueprintAssignable)
	FOnItemDraggedDelegate OnItemDraggedDelegate;
};
