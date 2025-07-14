// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionLibrary/WidgetControllerFunctionLibrary.h"

#include "Kismet/GameplayStatics.h"
#include "Player/WitchPTPlayerState.h"
#include "UI/HUD/WitchPTHUD.h"
#include "UI/WidgetControllers/WitchPTWidgetController.h"

UInventoryWidgetController* UWidgetControllerFunctionLibrary::SetInventoryWidgetController(
	const UObject* WorldContextObject)
{
    if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AWitchPTHUD* WitchPTHUD = Cast<AWitchPTHUD>(PC->GetHUD()))
		{
			AWitchPTPlayerState* PS = PC->GetPlayerState<AWitchPTPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WCParams = FWidgetControllerParams(PC, PS,ASC, AS);
			
			return WitchPTHUD->SetInventoryWidgetController(WCParams);
		}
	}
	return nullptr;
}

UQuickBarWidgetController* UWidgetControllerFunctionLibrary::SetQuickBarWidgetController(
	const UObject* WorldContextObject)
{
    if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AWitchPTHUD* WitchPTHUD = Cast<AWitchPTHUD>(PC->GetHUD()))
		{
			AWitchPTPlayerState* PS = PC->GetPlayerState<AWitchPTPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WCParams = FWidgetControllerParams(PC, PS,ASC, AS);
			
			return WitchPTHUD->SetQuickBarWidgetController(WCParams);
		}
	}
	return nullptr;
}

URitualWidgetController* UWidgetControllerFunctionLibrary::SetRitualWidgetController(const UObject* WorldContextObject)
{
    if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AWitchPTHUD* WitchPTHUD = Cast<AWitchPTHUD>(PC->GetHUD()))
		{
			AWitchPTPlayerState* PS = PC->GetPlayerState<AWitchPTPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WCParams = FWidgetControllerParams(PC, PS,ASC, AS);
			
			return WitchPTHUD->SetRitualWidgetController(WCParams);
		}
	}
	return nullptr;
}

UCauldronWidgetController* UWidgetControllerFunctionLibrary::SetCauldronWidgetController(
	const UObject* WorldContextObject)
{
    if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AWitchPTHUD* WitchPTHUD = Cast<AWitchPTHUD>(PC->GetHUD()))
		{
			AWitchPTPlayerState* PS = PC->GetPlayerState<AWitchPTPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WCParams = FWidgetControllerParams(PC, PS,ASC, AS);
			
			return WitchPTHUD->SetCauldronWidgetController(WCParams);
		}
	}
	return nullptr;
}

UOverlayWidgetController* UWidgetControllerFunctionLibrary::SetOverlayWidgetController(
	const UObject* WorldContextObject)
{
    if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AWitchPTHUD* WitchPTHUD = Cast<AWitchPTHUD>(PC->GetHUD()))
		{
			AWitchPTPlayerState* PS = PC->GetPlayerState<AWitchPTPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WCParams = FWidgetControllerParams(PC, PS,ASC, AS);
			
			return WitchPTHUD->SetOverlayWidgetController(WCParams);
		}
	}
	return nullptr;
}
