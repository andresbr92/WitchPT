// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/UI_GameUILayer.h"

#include "Components/Border.h"

void UUI_GameUILayer::NativeConstruct()
{
	Super::NativeConstruct();
	
	// Initialize the widget pool with the current world context
	WidgetPool.SetWorld(GetWorld());
}

UUserWidget* UUI_GameUILayer::PushContent(TSubclassOf<UUserWidget> WidgetClass)
{
	if (!WidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("PushContent: WidgetClass is null"));
		return nullptr;
	}

	if (!GetWorld())
	{
		UE_LOG(LogTemp, Warning, TEXT("PushContent: GetWorld() returned null"));
		return nullptr;
	}
	
	if (UUserWidget* UserWidgetInstance = WidgetPool.GetOrCreateInstance<UUserWidget>(WidgetClass))
	{
		// TODO: Check the creation of this widget by this tutorial: https://unreal-garden.com/tutorials/userwidget-pool/
		CollapseTop();
		Border->ClearChildren();
		Stack.Add(UserWidgetInstance);
		Border->AddChild(UserWidgetInstance);
		ShowTop();
		UE_LOG(LogTemp, Log, TEXT("Pushing widget: %s"), *UserWidgetInstance->GetName());
		return UserWidgetInstance;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PushContent: Failed to create widget instance for class: %s"), WidgetClass ? *WidgetClass->GetName() : TEXT("NULL"));
	}
	return nullptr;
}

void UUI_GameUILayer::ReleaseSlateResources(bool bReleaseChildren)
{
	WidgetPool.ReleaseAllSlateResources();
	Super::ReleaseSlateResources(bReleaseChildren);
}


void UUI_GameUILayer::PopContent()
{
	if (!Stack.IsEmpty())
	{
		if (UUserWidget* WidgetToRelease = Stack.Last())
		{
			WidgetToRelease->RemoveFromParent();
			Stack.Pop();
			WidgetPool.Release(WidgetToRelease);
		}
		
		if (UUserWidget* NewTopWidget = GetTopScreen())
		{
			NewTopWidget->SetVisibility(ESlateVisibility::Visible);
			Border->AddChild(NewTopWidget);
		}
	}
}

UUserWidget* UUI_GameUILayer::GetTopScreen() const
{
	if (!Stack.IsEmpty())
	{
		return Stack.Last();
	}
	return nullptr;
}

FText UUI_GameUILayer::GetStackListNames()
{
	// Return a list of the names but in reverse order
	FString ListNames;
	for (int i = Stack.Num() - 1; i >= 0; i--)
	{
		ListNames += Stack[i]->GetName() + "\n";
	}
	return FText::FromString(ListNames);
}

void UUI_GameUILayer::ClearStack()
{
	for (auto & Widget : Stack)
	{
		if (Widget)
		{
			Widget->RemoveFromParent();
		}
	}
	Stack.Empty();
	Border->ClearChildren();
	
	
}

void UUI_GameUILayer::CollapseTop()
{
	if (UUserWidget* TopWidgetToCollapse = GetTopScreen())
	{
		TopWidgetToCollapse->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUI_GameUILayer::ShowTop()
{
	if (UUserWidget* TopWidgetToShow = GetTopScreen())
	{
		TopWidgetToShow->SetVisibility(ESlateVisibility::Visible);
	}
}