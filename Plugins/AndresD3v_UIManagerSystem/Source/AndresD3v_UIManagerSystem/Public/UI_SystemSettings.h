// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "UI_SystemSettings.generated.h"

class UUI_GameUIPolicy;
/**
 * 
 */
UCLASS(Config = Game, defaultconfig, meta = (DisplayName = "AndresD3v GUI System Settings"))
class ANDRESD3V_UIMANAGERSYSTEM_API UUI_SystemSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	/**
	 * Retrieves the Generic UI System settings.
	 * @return The UI system settings.
	 */
	UFUNCTION(BlueprintPure, Category="WitchPT|Settings", meta = (DisplayName = "Get Generic UI System Settings"))
	static const UUI_SystemSettings* Get();

	/**
	 * Default UI policy class for the game layout.
	 */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="WitchPT|Settings")
	TSoftClassPtr<UUI_GameUIPolicy> GameUIPolicyClass;
};
