// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/Layer/UWitchPTUILayer.h"

#include "Components/Border.h"

UUserWidget* UUWitchPTUILayer::PushContent(TSoftClassPtr<UUserWidget> WidgetClass)
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
			ShowTop();
			return PushedWidget;
		}
	}
	return nullptr;
}

void UUWitchPTUILayer::PopContent()
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

UUserWidget* UUWitchPTUILayer::GetTopScreen() const
{
	if (!Stack.IsEmpty())
	{
		return Stack.Last();
	}
	return nullptr;
}

FText UUWitchPTUILayer::GetStackListNames()
{
	// Reverse for to get the names in the correct order
	FString StackNames;
	for (int32 i = Stack.Num() - 1; i >= 0; --i)
	{
		if (Stack[i])
		{
			StackNames += Stack[i]->GetName();
			if (i > 0)
			{
				StackNames += TEXT(", ");
			}
		}
	}
	return FText ::FromString(StackNames);
}

void UUWitchPTUILayer::ClearStack()
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

void UUWitchPTUILayer::CollapseTop()
{
	if (UUserWidget* TopWidgetToCollapse = GetTopScreen())
	{
		TopWidgetToCollapse->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUWitchPTUILayer::ShowTop()
{
	if (UUserWidget* TopWidgetToShow = GetTopScreen())
	{
		TopWidgetToShow->SetVisibility(ESlateVisibility::Visible);
	}
}
