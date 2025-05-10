// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WitchPTInventoryFunctionLibrary.generated.h"

class UWitchPTInventoryItemDefinition;
class UWitchPTInventoryItemFragment;
/**
 * 
 */
UCLASS()
class WITCHPT_API UWitchPTInventoryFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	UFUNCTION(BlueprintCallable, meta=(DeterminesOutputType=FragmentClass))
	static const UWitchPTInventoryItemFragment* FindItemDefinitionFragment(TSubclassOf<UWitchPTInventoryItemDefinition> ItemDef, TSubclassOf<UWitchPTInventoryItemFragment> FragmentClass);
};
