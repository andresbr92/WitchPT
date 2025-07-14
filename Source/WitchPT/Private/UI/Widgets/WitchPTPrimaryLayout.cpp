// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/WitchPTPrimaryLayout.h"

#include "FWitchPTGameplayTags.h"
#include "Subsystems/UIManagerSubsystem.h"
#include "UI/Widgets/Layer/WitchPTUILayer.h"

UWitchPTPrimaryLayout::UWitchPTPrimaryLayout()
{
	
	
}

void UWitchPTPrimaryLayout::NativeConstruct()
{
	Super::NativeConstruct();

	
}

bool UWitchPTPrimaryLayout::RegisterLayer(FGameplayTag LayerTag, UWitchPTUILayer* InLayer)
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

bool UWitchPTPrimaryLayout::UnRegisterLayer(FGameplayTag LayerTag)
{
	if (LayerTag.IsValid())
	{
		if (UWitchPTUILayer* LayerToRemove = Layers.FindRef(LayerTag))
		{
			LayerToRemove->RemoveFromParent();
			Layers.Remove(LayerTag);
			UE_LOG(LogTemp, Log, TEXT("Unregistered layer: %s"), *LayerTag.ToString());
			return true;
		}
	}
	return false;
}
UUserWidget* UWitchPTPrimaryLayout::PushContentToLayer(FGameplayTag LayerTag,
	TSoftClassPtr<UUserWidget> WidgetClass)
{
	if (LayerTag.IsValid() && WidgetClass.IsValid())
	{
		if (UWitchPTUILayer* Layer = Layers.FindRef(LayerTag))
		{
			UUserWidget* PushedWidget = Layer->PushContent(WidgetClass);
			return PushedWidget;
		}
	}
	return nullptr;
}

void UWitchPTPrimaryLayout::PopContentFromLayer(FGameplayTag LayerTag)
{
	if (LayerTag.IsValid())
	{
		if (UWitchPTUILayer* Layer = Layers.FindRef(LayerTag))
		{
			Layer->PopContent();
		}
		
	}
}

void UWitchPTPrimaryLayout::ClearAllLayers()
{
	for (auto& LayerPair : Layers)
	{
		if (LayerPair.Value)
		{
			LayerPair.Value->ClearStack();
		}
	}
}

void UWitchPTPrimaryLayout::ClearAllLayersExcept(FGameplayTag LayerTag)
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

void UWitchPTPrimaryLayout::ClearLayer(FGameplayTag LayerTag)
{
	if (LayerTag.IsValid())
	{
		if (UWitchPTUILayer* Layer = Layers.FindRef(LayerTag))
		{
			Layer->ClearStack();
		}
		
	}
}

void UWitchPTPrimaryLayout::PushInitialScreens()
{
	if (InitialScreens.Num())
	{
		for (auto & ScreenPair : InitialScreens)
		{
			if (ScreenPair.Value.IsValid())
			{
				if (ULocalPlayer* LocalPlayer = GetOwningLocalPlayer())
				{
					if (UUIManagerSubsystem* UIManagerSubsystem = LocalPlayer->GetSubsystem<UUIManagerSubsystem>())
					{
						UIManagerSubsystem->PushContentToLayer(ScreenPair.Key, ScreenPair.Value);
					}
				}
			}
		}
	}
}
void UWitchPTPrimaryLayout::ShowFirstElementOfLayer(FGameplayTag LayerTag)
{
	if (LayerTag.IsValid())
	{
		if (UWitchPTUILayer* Layer = Layers.FindRef(LayerTag))
		{
			Layer->ShowTop();
		}
	}
}
void UWitchPTPrimaryLayout::CollapseFirstElementOfLayer(FGameplayTag LayerTag)
{
	if (LayerTag.IsValid())
	{
		if (UWitchPTUILayer* Layer = Layers.FindRef(LayerTag))
		{
			Layer->CollapseTop();
		}
	}
}