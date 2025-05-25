// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Ritual/RitualFunctionLibrary.h"

#include "Kismet/GameplayStatics.h"
#include "Player/WitchPTPlayerState.h"
#include "UI/HUD/WitchPTHUD.h"
#include "UI/WidgetControllers/WitchPTWidgetController.h"

URitualWidgetController* URitualFunctionLibrary::SetRitualWidgetController(const UObject* WorldContextObject)
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

UWitchPTWidgetController* URitualFunctionLibrary::SetCauldronWidgetController(const UObject* WorldContextObject)
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
