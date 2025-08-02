// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/Layer/WitchPTUILayer.h"

#include "Blueprint/UserWidgetPool.h"
#include "Components/Border.h"
#include "Subsystems/UIManagerSubsystem.h"

UUserWidget* UWitchPTUILayer::PushContent(TSubclassOf<UUserWidget> WidgetClass)
{
	if (WidgetClass)
	{
		
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
		
	}
	return nullptr;
}

void UWitchPTUILayer::ReleaseSlateResources(bool bReleaseChildren)
{
	WidgetPool.ReleaseAllSlateResources();
	Super::ReleaseSlateResources(bReleaseChildren);
}


void UWitchPTUILayer::PopContent()
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

UUserWidget* UWitchPTUILayer::GetTopScreen() const
{
	if (!Stack.IsEmpty())
	{
		return Stack.Last();
	}
	return nullptr;
}

FText UWitchPTUILayer::GetStackListNames()
{
	// Return a list of the names but in reverse order
	FString ListNames;
	for (int i = Stack.Num() - 1; i >= 0; i--)
	{
		ListNames += Stack[i]->GetName() + "\n";
	}
	return FText::FromString(ListNames);
}

FText UWitchPTUILayer::GetStackWidgetControllerName()
{
	FString ListNames;
	for (int i = Stack.Num() - 1; i >= 0; i--)
	{
		UWitchPTUserWidget* WitchWidget = Cast<UWitchPTUserWidget>(Stack[i]);
		// also print his WidgetController name
		if (WitchWidget && WitchWidget->WidgetController)
		{
			ListNames += WitchWidget->WidgetController->GetClass()->GetName() + " - ";
			
		}
		
	}
	return FText::FromString(ListNames);
}

void UWitchPTUILayer::ClearStack()
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

void UWitchPTUILayer::CollapseTop()
{
	if (UUserWidget* TopWidgetToCollapse = GetTopScreen())
	{
		TopWidgetToCollapse->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UWitchPTUILayer::ShowTop()
{
	if (UUserWidget* TopWidgetToShow = GetTopScreen())
	{
		TopWidgetToShow->SetVisibility(ESlateVisibility::Visible);
	}
}
