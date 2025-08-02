// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/Layer/WitchPTUILayer.h"

#include "Components/Border.h"
#include "Subsystems/UIManagerSubsystem.h"

UUserWidget* UWitchPTUILayer::PushContent(UUserWidget* InUserWidget)
{
	if (IsValid(InUserWidget))
	{
		PushedWidget = InUserWidget;
		CollapseTop();
		Border->ClearChildren();
		Stack.Add(InUserWidget);
		Border->AddChild(InUserWidget);
		ShowTop();
		UE_LOG(LogTemp, Log, TEXT("Pushing widget: %s"), *InUserWidget->GetName());
		return PushedWidget;
		
	}
	return nullptr;
}



void UWitchPTUILayer::PopContent()
{
	if (!Stack.IsEmpty())
	{
		UUserWidget* TopWidgetToRelease = Stack.Last();
		if (TopWidgetToRelease)
		{
			TopWidgetToRelease->RemoveFromParent();
			Stack.Pop();

			// --- Cambio Clave ---
			// En lugar de dejar que se destruya, lo devolvemos al pool.
			if (ULocalPlayer* LocalPlayer = GetOwningLocalPlayer())
			{
				if (UUIManagerSubsystem* UIManagerSubsystem = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>())
				{
					UIManagerSubsystem->ReleaseWidgetToPool(TopWidgetToRelease);
				}
			}
		}

		// Mostrar el siguiente widget en el stack si existe
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
