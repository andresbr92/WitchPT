// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WitchPT_GameUIPolicy.h"

#include "UI/GameUIContexts/WitchPT_GameUIContextBase.h"
#include "UI/Widgets/WitchPTPrimaryLayout.h"

UWitchPTPrimaryLayout* UWitchPT_GameUIPolicy::GetRootLayout(const ULocalPlayer* LocalPlayer) const
{
	if (const FRootViewportLayoutInfo* LayoutInfo = RootViewportLayouts.FindByKey(LocalPlayer))
	{
		return LayoutInfo->RootLayout;
	}
	return nullptr;
}

void UWitchPT_GameUIPolicy::CreateLayoutWidget(ULocalPlayer* LocalPlayer)
{
	if (APlayerController* PlayerController = LocalPlayer->GetPlayerController(GetWorld()))
	{
		TSubclassOf<UWitchPTPrimaryLayout> LayoutWidgetClass = PrimaryLayoutClass.LoadSynchronous();
		if (ensure(LayoutWidgetClass && !LayoutWidgetClass->HasAnyClassFlags(CLASS_Abstract)))
		{
			UWitchPTPrimaryLayout* NewLayoutObject = CreateWidget<UWitchPTPrimaryLayout>(PlayerController, LayoutWidgetClass);
			RootViewportLayouts.Emplace(LocalPlayer, NewLayoutObject, true);

			AddLayoutToViewport(LocalPlayer, NewLayoutObject);
		}
	}
	
}

void UWitchPT_GameUIPolicy::AddLayoutToViewport(ULocalPlayer* LocalPlayer, UWitchPTPrimaryLayout* Layout)
{
	UE_LOG(LogTemp, Log, TEXT("[%s] is adding player [%s]'s to the viewport"), *GetName(), *GetNameSafe(LocalPlayer));
	Layout->SetPlayerContext(LocalPlayer);
	Layout->AddToPlayerScreen(1000);
}

void UWitchPT_GameUIPolicy::RemoveLayoutFromViewport(ULocalPlayer* LocalPlayer, UWitchPTPrimaryLayout* Layout)
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

void UWitchPT_GameUIPolicy::NotifyPlayerAdded(ULocalPlayer* LocalPlayer)
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

void UWitchPT_GameUIPolicy::NotifyPlayerRemoved(ULocalPlayer* LocalPlayer)
{
	if (FRootViewportLayoutInfo* LayoutInfo = RootViewportLayouts.FindByKey(LocalPlayer))
	{
		RemoveLayoutFromViewport(LocalPlayer, LayoutInfo->RootLayout);
		LayoutInfo->bIsAddedToViewport = false;
		
	}
}

void UWitchPT_GameUIPolicy::AddContext(const ULocalPlayer* LocalPlayer,
	UWitchPT_GameUIContextBase* NewContext)
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

void UWitchPT_GameUIPolicy::RemoveContext(const ULocalPlayer* LocalPlayer,
	TSubclassOf<UWitchPT_GameUIContextBase> ContextClass)
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

UWitchPT_GameUIContextBase* UWitchPT_GameUIPolicy::GetContext(const ULocalPlayer* LocalPlayer,
                                                              TSubclassOf<UWitchPT_GameUIContextBase> ContextClass)
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

UUIManagerSubsystem* UWitchPT_GameUIPolicy::GetOwningSubsystem() const
{
	return nullptr;
}

UWorld* UWitchPT_GameUIPolicy::GetWorld() const
{
	return UObject::GetWorld();
}
