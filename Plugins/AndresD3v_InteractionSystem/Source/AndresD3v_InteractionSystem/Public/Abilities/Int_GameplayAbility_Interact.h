// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Components/Int_InteractionSystemComponent.h"

#include "Int_GameplayAbility_Interact.generated.h"

struct FSmartObjectClaimHandle;
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class ANDRESD3V_INTERACTIONSYSTEM_API UInt_GameplayAbility_Interact : public UGameplayAbility
{
	GENERATED_BODY()
public:
	
	UInt_GameplayAbility_Interact();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
protected:
	UFUNCTION(BlueprintCallable, Category="GCS|Interaction", meta=(ExpandBoolAsExecs=ReturnValue))
	bool TryClaimInteraction(int32 Index, FSmartObjectClaimHandle& ClaimedHandle);
	UFUNCTION(BlueprintNativeEvent, Category="GCS|Interaction")
	void OnInteractActorChanged(AActor* OldActor, AActor* NewActor);

	UPROPERTY(BlueprintReadOnly, Category="AndresD3v|Interaction")
	TObjectPtr<UInt_InteractionSystemComponent> InteractionSystem{ nullptr };
};
