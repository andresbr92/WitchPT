// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/UI_GameUILayout.h"

#include "Widgets/UI_GameUILayer.h"


UUI_GameUILayout::UUI_GameUILayout(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UUI_GameUILayout::NativeConstruct()
{
	Super::NativeConstruct();

	
}

bool UUI_GameUILayout::RegisterLayer(FGameplayTag LayerTag, UUI_GameUILayer* InLayer)
{
	
	if (InLayer != nullptr && LayerTag.IsValid())
	{
		if (Layers.Contains(LayerTag))
		{
			UE_LOG(LogTemp, Warning, TEXT("Layer %s is already registered!"), *LayerTag.ToString());
			return false;
		}
		
		// Register the layer
		Layers.Add(LayerTag, InLayer);
		UE_LOG(LogTemp, Log, TEXT("Registered layer: %s"), *LayerTag.ToString());
		
		return true;
	}
	return false;
}

bool UUI_GameUILayout::UnRegisterLayer(FGameplayTag LayerTag)
{
	if (LayerTag.IsValid())
	{
		if (UUI_GameUILayer* LayerToRemove = Layers.FindRef(LayerTag))
		{
			LayerToRemove->RemoveFromParent();
			Layers.Remove(LayerTag);
			UE_LOG(LogTemp, Log, TEXT("Unregistered layer: %s"), *LayerTag.ToString());
			return true;
		}
	}
	return false;
}
UUserWidget* UUI_GameUILayout::PushContentToLayer(FGameplayTag LayerTag, TSubclassOf<UUserWidget> WidgetClass)
{
	if (LayerTag.IsValid() && WidgetClass)
	{
		if (UUI_GameUILayer* Layer = Layers.FindRef(LayerTag))
		{
			UUserWidget* PushedWidget = Layer->PushContent(WidgetClass);
			return PushedWidget;
		}
	}
	return nullptr;
}

void UUI_GameUILayout::PopContentFromLayer(FGameplayTag LayerTag)
{
	if (LayerTag.IsValid())
	{
		if (UUI_GameUILayer* Layer = Layers.FindRef(LayerTag))
		{
			Layer->PopContent();
		}
		
	}
}

void UUI_GameUILayout::ClearAllLayers()
{
	for (auto& LayerPair : Layers)
	{
		if (LayerPair.Value)
		{
			LayerPair.Value->ClearStack();
		}
	}
}

void UUI_GameUILayout::ClearAllLayersExcept(FGameplayTag LayerTag)
{
	if (LayerTag.IsValid())
	{
		for (auto& LayerPair : Layers)
		{
			if (LayerPair.Key != LayerTag && LayerPair.Value)
			{
				LayerPair.Value->ClearStack();
			}
		}
	}
}

void UUI_GameUILayout::ClearLayer(FGameplayTag LayerTag)
{
	if (LayerTag.IsValid())
	{
		if (UUI_GameUILayer* Layer = Layers.FindRef(LayerTag))
		{
			Layer->ClearStack();
		}
		
	}
}
