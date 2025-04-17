// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "WitchPTAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class WITCHPT_API UWitchPTAssetManager : public UAssetManager
{
	GENERATED_BODY()
public:
	static UWitchPTAssetManager& Get();
protected:
	virtual void StartInitialLoading() override;

	
};
