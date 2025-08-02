// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "GenericUISystemSettings.generated.h"

class UWitchPT_GameUIPolicy;
/**
 * 
 */
UCLASS(Config = Game, defaultconfig, meta = (DisplayName = "Generic UI System Settings"))
class WITCHPT_API UGenericUISystemSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	/**
	 * Retrieves the Generic UI System settings.
	 * @return The UI system settings.
	 */
	UFUNCTION(BlueprintPure, Category="WitchPT|Settings", meta = (DisplayName = "Get Generic UI System Settings"))
	static const UGenericUISystemSettings* Get();

	/**
	 * Default UI policy class for the game layout.
	 */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="WitchPT|Settings")
	TSoftClassPtr<UWitchPT_GameUIPolicy> GameUIPolicyClass;
};
