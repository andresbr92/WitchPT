// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PositionInterface.generated.h"

class ARitualAltar;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPositionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class WITCHPT_API IPositionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Mechanics|Position|Query")
	ARitualAltar* GetRitualAltarActor() const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Mechanics|Position|Query")
	bool IsOccupied();
};
