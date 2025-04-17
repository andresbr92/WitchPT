// Fill out your copyright notice in the Description page of Project Settings.


#include "WitchPTAssetManager.h"

#include "AbilitySystemGlobals.h"
#include "FWitchPTGameplayTags.h"

UWitchPTAssetManager& UWitchPTAssetManager::Get()
{
	check(GEngine)
	UWitchPTAssetManager* WitchPtAssetManager = Cast<UWitchPTAssetManager>(GEngine->AssetManager);
	return *WitchPtAssetManager;
}

void UWitchPTAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	FWitchPTGameplayTags::InitializeGameplayTags();
	/**
	 * 
	 */
	// TODO: What is this function for? 

	UAbilitySystemGlobals::Get().InitGlobalData();
	
}
