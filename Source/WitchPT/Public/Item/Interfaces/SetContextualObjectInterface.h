// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SetContextualObjectInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class USetContextualObjectInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class WITCHPT_API ISetContextualObjectInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION()
	virtual void SetContextualObject(UObject* ContextualObject);
	
};
