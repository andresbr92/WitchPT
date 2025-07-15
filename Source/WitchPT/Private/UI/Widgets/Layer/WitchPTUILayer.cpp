// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/Layer/WitchPTUILayer.h"

#include "Components/Border.h"

UUserWidget* UWitchPTUILayer::PushContent(TSoftClassPtr<UUserWidget> WidgetClass)
{
	if (WidgetClass.IsValid())
	{
		if(UClass* LoadedClass = WidgetClass.Get())
		{
			UUserWidget* NewUserWidgetInstance = CreateWidget<UUserWidget>(GetOwningPlayer(), LoadedClass);
			PushedWidget = NewUserWidgetInstance;
			CollapseTop();
			Border->ClearChildren();
			Stack.Add(NewUserWidgetInstance);
			Border->AddChild(NewUserWidgetInstance);
			ShowTop();
			UE_LOG(LogTemp, Log, TEXT("Pushing widget: %s"), *NewUserWidgetInstance->GetName());
			return PushedWidget;
		}
	}
	return nullptr;
}

void UWitchPTUILayer::PopContent()
{
	if (!Stack.IsEmpty())
	{
		TopWidget = GetTopScreen();
		if (TopWidget)
		{
			TopWidget->RemoveFromParent();
			// remove the top widget from the stack
			Stack.RemoveAt(Stack.Num() - 1);
			Border->ClearChildren();
			TopWidget = GetTopScreen();
			if (TopWidget)
			{
				TopWidget->SetVisibility(ESlateVisibility::Visible);
				Border->AddChild(TopWidget);
			}
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
