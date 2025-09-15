// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTask.h"
#include "SmartObjectRuntime.h"
#include "SmartObjectTypes.h"
#include "Abilities/GameplayAbility.h"
#include "Int_GameplayTask_UseSmartObjectWithGameplayBehaviour.generated.h"

class UGameplayBehavior;
struct FSmartObjectClaimHandle;
/**
 * 
 */
UCLASS()
class ANDRESD3V_INTERACTIONSYSTEM_API UInt_GameplayTask_UseSmartObjectWithGameplayBehaviour : public UGameplayTask
{
	GENERATED_BODY()
public:
	UInt_GameplayTask_UseSmartObjectWithGameplayBehaviour(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "AndresD3v|Interaction", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UInt_GameplayTask_UseSmartObjectWithGameplayBehaviour* UseSmartObjectWithGameplayBehavior(UGameplayAbility* OwningAbility, FSmartObjectClaimHandle ClaimHandle,
																								   ESmartObjectClaimPriority ClaimPriority = ESmartObjectClaimPriority::Normal);
	/**
	 * Sets the smart object claim handle
	 * @param Handle The claim handle
	 */
	void SetClaimHandle(const FSmartObjectClaimHandle& Handle) { ClaimedHandle = Handle; }
protected:
	virtual void Activate() override;
	virtual void OnDestroy(bool bInOwnerFinished) override;

	/**
	 * Starts the interaction with the smart object.
	 * @return True if the interaction started successfully.
	 */
	bool StartInteraction();

	/**
	 * Called when the behavior finishes.
	 * @param Behavior The gameplay behavior
	 * @param Avatar The avatar actor
	 * @param bInterrupted Whether the behavior was interrupted.
	 */
	void OnSmartObjectBehaviorFinished(UGameplayBehavior* Behavior, AActor* Avatar, const bool bInterrupted);

	/**
	 * Called when the smart object slot is invalidated.
	 * @param ClaimHandle The claim handle. 认领句柄。
	 * @param State The slot state. 槽状态。
	 */
	void OnSlotInvalidated(const FSmartObjectClaimHandle& ClaimHandle, const ESmartObjectSlotState State);
	
	/**
	 * Delegate for when the interaction succeeds.
	 */
	UPROPERTY(BlueprintAssignable)
	FGenericGameplayTaskDelegate OnSucceeded;

	/**
	 * Delegate for when the interaction fails.
	 */
	UPROPERTY(BlueprintAssignable)
	FGenericGameplayTaskDelegate OnFailed;
	
	/**
	 * The gameplay behavior for the interaction.
	 */
	UPROPERTY()
	TObjectPtr<UGameplayBehavior> GameplayBehavior;
	
	/**
	 * The claimed smart object handle
	 */
	FSmartObjectClaimHandle ClaimedHandle;
	
	/**
	 * Delegate handle for behavior-finished notification.
	 */
	FDelegateHandle OnBehaviourFinishedNotifyHandle;
	
	/**
	 * Indicates if the behavior has finished.
	 */
	bool bBehaviorFinished;
};
