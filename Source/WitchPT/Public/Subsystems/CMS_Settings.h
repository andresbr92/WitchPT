// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "CMS_Settings.generated.h"

/**
 * 
 */
UCLASS(Config= Game, defaultconfig, meta = (DisplayName = "CMS Settings"))
class WITCHPT_API UCMS_Settings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category="WitchPT|CMS|Settings", meta = (DisplayName = "Get CMS Settings"))
	static const UCMS_Settings* Get();

	UPROPERTY(Config, EditDefaultsOnly, Category="WitchPT|CMS|Settings")
	const TSoftObjectPtr<UDataTable> PotionRecipesTableRef;
	
};
