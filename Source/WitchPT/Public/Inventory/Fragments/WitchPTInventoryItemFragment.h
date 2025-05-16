// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "WitchPTInventoryItemFragment.generated.h"

template <typename T> class TSubclassOf;

class UWitchPTInventoryItemInstance;
/**
 * 
 */
UCLASS(DefaultToInstanced, EditInlineNew, Abstract)
class WITCHPT_API UWitchPTInventoryItemFragment : public UObject
{
	GENERATED_BODY()

public:
	virtual void OnInstanceCreated(UWitchPTInventoryItemInstance* Instance) const {}
	
};
