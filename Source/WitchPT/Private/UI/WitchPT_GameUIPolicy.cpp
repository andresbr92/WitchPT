// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WitchPT_GameUIPolicy.h"

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

UUIManagerSubsystem* UWitchPT_GameUIPolicy::GetOwningSubsystem() const
{
	return nullptr;
}

UWorld* UWitchPT_GameUIPolicy::GetWorld() const
{
	return UObject::GetWorld();
}
