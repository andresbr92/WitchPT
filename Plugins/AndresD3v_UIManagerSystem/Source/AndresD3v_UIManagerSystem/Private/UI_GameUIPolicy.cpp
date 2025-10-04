// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_GameUIPolicy.h"

#include "UI_GameUIContext.h"
#include "Blueprint/UserWidget.h"
#include "UI_ManagerSubsystem.h"
#include "Widgets/UI_GameUILayout.h"

UUI_GameUILayout* UUI_GameUIPolicy::GetRootLayout(const ULocalPlayer* LocalPlayer) const
{
	if (const FRootViewportLayoutInfo* LayoutInfo = RootViewportLayouts.FindByKey(LocalPlayer))
	{
		return LayoutInfo->RootLayout;
	}
	return nullptr;
}

void UUI_GameUIPolicy::CreateLayoutWidget(ULocalPlayer* LocalPlayer)
{
	if (APlayerController* PlayerController = LocalPlayer->GetPlayerController(GetWorld()))
	{
		TSubclassOf<UUI_GameUILayout> LayoutWidgetClass = PrimaryLayoutClass.LoadSynchronous();
		if (ensure(LayoutWidgetClass && !LayoutWidgetClass->HasAnyClassFlags(CLASS_Abstract)))
		{
			UUI_GameUILayout* NewLayoutObject = CreateWidget<UUI_GameUILayout>(PlayerController, LayoutWidgetClass);
			RootViewportLayouts.Emplace(LocalPlayer, NewLayoutObject, true);

			AddLayoutToViewport(LocalPlayer, NewLayoutObject);
		}
	}
	
}

void UUI_GameUIPolicy::AddLayoutToViewport(ULocalPlayer* LocalPlayer, UUI_GameUILayout* Layout)
{
	UE_LOG(LogTemp, Log, TEXT("[%s] is adding player [%s]'s to the viewport"), *GetName(), *GetNameSafe(LocalPlayer));
	Layout->SetPlayerContext(LocalPlayer);
	Layout->AddToPlayerScreen(1000);
}

void UUI_GameUIPolicy::RemoveLayoutFromViewport(ULocalPlayer* LocalPlayer, UUI_GameUILayout* Layout)
{
	TWeakPtr<SWidget> LayoutSlateWidget = Layout->GetCachedWidget();
	if (LayoutSlateWidget.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("[%s] is removing player [%s]'s root layout [%s] from the viewport"), *GetName(), *GetNameSafe(LocalPlayer), *GetNameSafe(Layout));

		Layout->RemoveFromParent();
		if (LayoutSlateWidget.IsValid())
		{
			UE_LOG(LogTemp, Log, TEXT("Player [%s]'s root layout [%s] has been removed from the viewport, but other references to its underlying Slate widget still exist. Noting in case we leak it."),
				   *GetNameSafe(LocalPlayer), *GetNameSafe(Layout));
		}

		
	}
}

void UUI_GameUIPolicy::NotifyPlayerAdded(ULocalPlayer* LocalPlayer)
{
	NotifyPlayerRemoved(LocalPlayer);

	if (FRootViewportLayoutInfo* LayoutInfo = RootViewportLayouts.FindByKey(LocalPlayer))
	{
		AddLayoutToViewport(LocalPlayer, LayoutInfo->RootLayout);
		LayoutInfo->bIsAddedToViewport = true;
	} else
	{
		CreateLayoutWidget(LocalPlayer);
	}
}

void UUI_GameUIPolicy::NotifyPlayerRemoved(ULocalPlayer* LocalPlayer)
{
	if (FRootViewportLayoutInfo* LayoutInfo = RootViewportLayouts.FindByKey(LocalPlayer))
	{
		RemoveLayoutFromViewport(LocalPlayer, LayoutInfo->RootLayout);
		LayoutInfo->bIsAddedToViewport = false;
		
	}
}

void UUI_GameUIPolicy::AddContext(const ULocalPlayer* LocalPlayer,
	UUI_GameUIContext* NewContext)
{
	if (FRootViewportLayoutInfo* LayoutInfo = RootViewportLayouts.FindByKey(LocalPlayer))
	{
		if (UObject* ExistingContext = GetContext(LocalPlayer, NewContext->GetClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("[%s] is trying to add repeat context of type(%s) for %s, which is not allowed!"), *GetName(), *NewContext->GetName(), *GetNameSafe(LocalPlayer));
			return;
		}
		LayoutInfo->Contexts.Add(NewContext);
		UE_LOG(LogTemp, Warning, TEXT("[%s] registered context of type(%s) for %s."), *GetName(), *NewContext->GetClass()->GetName(), *GetNameSafe(LocalPlayer));
	}
	
}

void UUI_GameUIPolicy::RemoveContext(const ULocalPlayer* LocalPlayer,
	TSubclassOf<UUI_GameUIContext> ContextClass)
{
	if (FRootViewportLayoutInfo* LayoutInfo = RootViewportLayouts.FindByKey(LocalPlayer))
	{
		int32 FoundContextIndex = INDEX_NONE;
		for ( int32 i = 0; i < LayoutInfo->Contexts.Num(); i++)
		{
			if (LayoutInfo->Contexts[i] && LayoutInfo->Contexts[i]->GetClass() == ContextClass)
			{
				FoundContextIndex = i;
				UE_LOG(LogTemp, Warning, TEXT("[%s] removed context of type(%s) for %s."), *GetName(), *ContextClass->GetName(), *GetNameSafe(LocalPlayer));
				break;
			}
		}
		LayoutInfo->Contexts.RemoveAt(FoundContextIndex);
	}
}

UUI_GameUIContext* UUI_GameUIPolicy::GetContext(const ULocalPlayer* LocalPlayer,
                                                              TSubclassOf<UUI_GameUIContext> ContextClass)
{
	if (const FRootViewportLayoutInfo* LayoutInfo = RootViewportLayouts.FindByKey(LocalPlayer))
	{
		for (int32 i = 0; i < LayoutInfo->Contexts.Num(); i++)
		{
			if (LayoutInfo->Contexts[i] && LayoutInfo->Contexts[i]->GetClass() == ContextClass)
			{
				return LayoutInfo->Contexts[i];
			}
		}
		
		
	}
	return nullptr;
}

UUI_ManagerSubsystem* UUI_GameUIPolicy::GetOwningSubsystem() const
{
	return nullptr;
}


UWorld* UUI_GameUIPolicy::GetWorld() const
{
	return UObject::GetWorld();
}
