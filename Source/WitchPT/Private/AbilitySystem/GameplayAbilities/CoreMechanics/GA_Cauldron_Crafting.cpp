// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayAbilities/CoreMechanics/GA_Cauldron_Crafting.h"

#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Item/BaseInteractableAltar.h"

void UGA_Cauldron_Crafting::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                            const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	

}

void UGA_Cauldron_Crafting::OnCharacterPositioned(FGameplayEventData Payload)
{
	UE_LOG(LogTemp, Warning, TEXT("OnCharacterPositioned called with Payload: %s"), *Payload.EventTag.ToString());
}

void UGA_Cauldron_Crafting::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	FAbilityTriggerData AbilityTrigger;
	AbilityTrigger.TriggerTag = FGameplayTag::RequestGameplayTag(FName("Ability.Interaction.Activate"));
	AbilityTrigger.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(AbilityTrigger);
	
	if (UAbilityTask_WaitGameplayEvent* WaitGameplayEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FGameplayTag::RequestGameplayTag(FName("Event.Cauldron.CharacterPositioned")), nullptr, true, true))
	{
		WaitGameplayEventTask->EventReceived.AddDynamic(this, &UGA_Cauldron_Crafting::OnCharacterPositioned);
		
	}
}
