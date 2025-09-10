// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WitchPTInventoryItemFragment.h"
#include "InventoryFragment_PickUpIcon.generated.h"

/**
 * 
 */
UCLASS()
class WITCHPT_API UInventoryFragment_PickUpIcon : public UWitchPTInventoryItemFragment
{
	GENERATED_BODY()
public:
	UInventoryFragment_PickUpIcon();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	TObjectPtr<USkeletalMesh> SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	FLinearColor PadColor;
};
