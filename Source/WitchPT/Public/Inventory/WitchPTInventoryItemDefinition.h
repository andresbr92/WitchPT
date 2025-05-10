// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WitchPTInventoryItemDefinition.generated.h"

class UWitchPTInventoryItemFragment;
struct FFrame;
/**
 * 
 */
UCLASS(Blueprintable, Const, Abstract)
class WITCHPT_API UWitchPTInventoryItemDefinition : public UObject
{
	GENERATED_BODY()
public:
	UWitchPTInventoryItemDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Display, Instanced)
	TArray<TObjectPtr<UWitchPTInventoryItemFragment>> Fragments;

	const UWitchPTInventoryItemFragment* FindFragmentByClass(TSubclassOf<UWitchPTInventoryItemFragment> FragmentClass) const;
};
