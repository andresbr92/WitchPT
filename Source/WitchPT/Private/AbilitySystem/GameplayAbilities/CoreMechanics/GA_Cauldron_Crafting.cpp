// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayAbilities/CoreMechanics/GA_Cauldron_Crafting.h"

#include "AbilitySystemComponent.h"
#include "FWitchPTGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystem/WitchPTAbilitySystemLibrary.h"
#include "GameFramework/Character.h"
#include "Item/BaseInteractableAltar.h"
#include "Item/CauldronAltar.h"
#include "Subsystems/UIManagerSubsystem.h"

void UGA_Cauldron_Crafting::OnCancelParticipatingInCauldron(FGameplayEventData Payload)
{
	// TODO: // Implement the logic to handle the cancellation of participation in the cauldron

	
	// const ACauldronAltar* CauldronAltar = Cast<ACauldronAltar>(CurrentCauldronAltar);
	// if (const TScriptInterface<IMechanicsInterface> MechanicsInterface = UWitchPTAbilitySystemLibrary::GetMechanicsInterface(CurrentCauldronAltar))
	// {
	// 	MechanicsInterface->RequestStartBrewingPotion_Implementation(const_cast<ACauldronAltar*> (CauldronAltar));
	// }
	// // remove gameplay effects
	// UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	// for (const FActiveGameplayEffectHandle& EffectHandle : ActiveEffectsHandles)
	// {
	// 	if (AbilitySystemComponent && EffectHandle.IsValid())
	// 	{
	// 		AbilitySystemComponent->RemoveActiveGameplayEffect(EffectHandle);
	// 	}
	// }
	// ActiveEffectsHandles.Empty();
	// if (UWorld* World = GetWorld())
	// {
	// 	if (UUIManagerSubsystem* UIManager = World->GetGameInstance()->GetSubsystem<UUIManagerSubsystem>())
	// 	{
	// 		const ACharacter* Character = Cast<ACharacter>(Payload.Instigator);
	// 		if (const APlayerController* PlayerController = Character ? Character->GetController<APlayerController>() : nullptr)
	// 		{
	// 			UIManager->PopContentFromLayer_ForPlayer(PlayerController, FWitchPTGameplayTags::Get().UI_Layer_Game);
	// 		}
	// 	}
	// }
}

void UGA_Cauldron_Crafting::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                            const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    
	const FGameplayTag CharacterPositionedTag = FWitchPTGameplayTags::Get().Event_Cauldron_CharacterPositioned;
	const FGameplayTag CancelParticipatingTag = FWitchPTGameplayTags::Get().Ability_Cauldron_CancelParticipatingInCauldron;
	const FGameplayTag GameLayer = FWitchPTGameplayTags::Get().UI_Layer_Game;
	
		
	const ACauldronAltar* CauldronAltar = Cast<ACauldronAltar>(TriggerEventData->Target);
	// store a general variable for the CauldronAltar 
	CurrentCauldronAltar = CauldronAltar;

	
	
	
	const ACharacter* Character = Cast<ACharacter>(TriggerEventData->Instigator);
	

	if (!CauldronAltar || !Character)
	{
		UE_LOG(LogTemp, Warning, TEXT("UGA_Cauldron_Crafting::ActivateAbility: Invalid Cauldron or Character"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}
	

    // Also keep listening for subsequent occurrences of the event during this ability's lifetime
    if (UAbilityTask_WaitGameplayEvent* WaitGameplayEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, CharacterPositionedTag, nullptr, true, true))
    {
        WaitGameplayEventTask->EventReceived.AddDynamic(this, &UGA_Cauldron_Crafting::OnCharacterPositioned);
        WaitGameplayEventTask->ReadyForActivation();
    }

	if (const TScriptInterface<IMechanicsInterface> MechanicsInterface = UWitchPTAbilitySystemLibrary::GetMechanicsInterface(TriggerEventData->Instigator))
	{
		MechanicsInterface->RequestStartBrewingPotion_Implementation(const_cast<ACauldronAltar*> (CauldronAltar));
		
	}

	if (UAbilityTask_WaitGameplayEvent* CancelParticipatingGameplayEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, CancelParticipatingTag, nullptr, true, true))
	{
		CancelParticipatingGameplayEventTask->EventReceived.AddDynamic(this, &UGA_Cauldron_Crafting::OnCancelParticipatingInCauldron);
		CancelParticipatingGameplayEventTask->ReadyForActivation();
	}
	
	if (Character->IsLocallyControlled())
	{
		if (UWorld* World = ActorInfo->OwnerActor->GetWorld())
		{
			if (UGameInstance* GameInstance = World->GetGameInstance())
			{
				if (UUIManagerSubsystem* UIManager = GameInstance->GetSubsystem<UUIManagerSubsystem>())
				{
					UIManager->PushContentToLayer_ForPlayer(ActorInfo->PlayerController.Get(), GameLayer, CauldronViewWidgetClass);
				}
			}
		}
		
	}

}

void UGA_Cauldron_Crafting::OnCharacterPositioned(FGameplayEventData Payload)
{
	UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	FGameplayEffectContextHandle GE_Blocked_EffectContext = AbilitySystemComponent->MakeEffectContext();
	GE_Blocked_EffectContext.AddSourceObject(this);
	ensure(CauldronBrewingEffectClass);
	
	FGameplayEffectSpecHandle GE_Blocked_EffectSpec = AbilitySystemComponent->MakeOutgoingSpec(CauldronBrewingEffectClass, 1.0f, GE_Blocked_EffectContext);
	FActiveGameplayEffectHandle SpecHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*GE_Blocked_EffectSpec.Data.Get());
	if (SpecHandle.IsValid())
	{
		ActiveEffectsHandles.Add(SpecHandle);
	}
	FGameplayEffectContextHandle GE_CauldronBrewing_EffectContext = AbilitySystemComponent->MakeEffectContext();
	GE_CauldronBrewing_EffectContext.AddSourceObject(this);
	ensure(StandardBlockedEffectClass);
	FGameplayEffectSpecHandle GE_CauldronBrewing_EffectSpec = AbilitySystemComponent->MakeOutgoingSpec(StandardBlockedEffectClass, 1.0f, GE_CauldronBrewing_EffectContext);
	FActiveGameplayEffectHandle StandardBlockedSpecHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*GE_CauldronBrewing_EffectSpec.Data.Get());
	if (StandardBlockedSpecHandle.IsValid())
	{
		ActiveEffectsHandles.Add(StandardBlockedSpecHandle);
	}
	
}

void UGA_Cauldron_Crafting::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	FAbilityTriggerData AbilityTrigger;
	AbilityTrigger.TriggerTag = FGameplayTag::RequestGameplayTag(FName("Ability.Interaction.Activate"));
	AbilityTrigger.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(AbilityTrigger);
}
