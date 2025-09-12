// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Engine/DataAsset.h"
#include "Int_InteractionDefinition.generated.h"

/**
 * 
 */
UCLASS()
class ANDRESD3V_INVENTORYSYSTEM_API UInt_InteractionDefinition : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	FText Text;
	UPROPERTY(EditDefaultsOnly)
	FText SubText;
};
