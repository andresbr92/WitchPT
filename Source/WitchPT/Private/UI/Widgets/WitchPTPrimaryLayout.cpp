// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/WitchPTPrimaryLayout.h"

#include "UI/Widgets/Layer/WitchPTUILayer.h"

UWitchPTPrimaryLayout::UWitchPTPrimaryLayout()
{
	// Todo: Register every layout in the map
}

bool UWitchPTPrimaryLayout::RegisterLayer(FGameplayTag LayoutTag, UWitchPTUILayer* InLayer)
{
	
	if (InLayer != nullptr && !LayoutTag.IsValid())
	{
		if (Layers.Contains(LayoutTag))
		{
			UE_LOG(LogTemp, Warning, TEXT("Layer %s is already registered!"), *LayoutTag.ToString());
			return false;
		}
		
		// Register the layer
		Layers.Add(LayoutTag, InLayer);
		UE_LOG(LogTemp, Log, TEXT("Registered layer: %s"), *LayoutTag.ToString());
		
		return true;
	}
	return false;
}

bool UWitchPTPrimaryLayout::UnRegisterLayer(FGameplayTag LayoutTag)
{
	if (LayoutTag.IsValid())
	{
		if (UWitchPTUILayer* LayerToRemove = Layers.FindRef(LayoutTag))
		{
			LayerToRemove->RemoveFromParent();
			Layers.Remove(LayoutTag);
			UE_LOG(LogTemp, Log, TEXT("Unregistered layer: %s"), *LayoutTag.ToString());
			return true;
		}
	}
	return false;
}
