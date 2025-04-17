// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "WitchPTInputConfig.generated.h"

USTRUCT(BlueprintType)
struct FWitchPTInputAction
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly)
	const class UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag = FGameplayTag();
};
/**
 * 
 */
UCLASS()
class WITCHPT_API UWitchPTInputConfig : public UDataAsset
{
	GENERATED_BODY()
	const UInputAction* GetAbilityByInputTag(FGameplayTag& InputTag, bool bLogNotFound)const ;
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FWitchPTInputAction> AbilityInputActions;
	
};
