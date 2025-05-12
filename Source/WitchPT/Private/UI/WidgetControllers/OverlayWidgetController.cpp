// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetControllers/OverlayWidgetController.h"

#include "AbilitySystem/WitchPTAbilitySystemComponent.h"
#include "AbilitySystem/WitchPTAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UWitchPTAttributeSet* WitchPT_AS = Cast<UWitchPTAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(WitchPT_AS->GetHealth());
	OnMaxHealthChanged.Broadcast(WitchPT_AS->GetMaxHealth());
	OnStaminaChanged.Broadcast(WitchPT_AS->GetStamina());
	OnMaxStaminaChanged.Broadcast(WitchPT_AS->GetMaxStamina());
	OnAgeChanged.Broadcast(WitchPT_AS->GetAge());
	OnMaxAgeChanged.Broadcast(WitchPT_AS->GetMaxAge());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UWitchPTAttributeSet* WitchPT_AS = Cast<UWitchPTAttributeSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(WitchPT_AS->GetHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
	{
		OnHealthChanged.Broadcast(Data.NewValue);
	});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(WitchPT_AS->GetMaxHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
	{
		OnMaxHealthChanged.Broadcast(Data.NewValue);
	});
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(WitchPT_AS->GetStaminaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
	{
		OnStaminaChanged.Broadcast(Data.NewValue);
	});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(WitchPT_AS->GetMaxStaminaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
	{
		OnMaxStaminaChanged.Broadcast(Data.NewValue);
	});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(WitchPT_AS->GetAgeAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
	{
		OnAgeChanged.Broadcast(Data.NewValue);
	});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(WitchPT_AS->GetMaxAgeAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
	{
		OnMaxAgeChanged.Broadcast(Data.NewValue);
	});

	UWitchPTAbilitySystemComponent* WitchPTAbilitySystemComponent = Cast<UWitchPTAbilitySystemComponent>(AbilitySystemComponent);
	
	WitchPTAbilitySystemComponent->OnEffectAssetTags.AddUObject(this, &UOverlayWidgetController::OnAssetTagReceivedFromGE);
	
	// Lambda for message delegates
	WitchPTAbilitySystemComponent->OnEffectAssetTags.AddLambda(
		// [this] is the capture list, in the case of this lambda, we want to capture this (OverlayWidgetController)
		[this](const FGameplayTagContainer& AssetTags)
	{
		for (const auto& Tag : AssetTags)
		{
			// Declare the tag we want to check. In our case "MessageTag"
			FGameplayTag ParentMessageGameplayTag = FGameplayTag::RequestGameplayTag(FName("Communication.Message"));
			if (Tag.MatchesTag(ParentMessageGameplayTag))
			{
				
					
				const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
				OnMessageWidgetRow.Broadcast(*Row);
				
			}
			
		}
	});

	

}
void UOverlayWidgetController::OnAssetTagReceivedFromGE(const FGameplayTagContainer& TagContainer) const
{

}
