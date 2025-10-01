// Fill out your copyright notice in the Description page of Project Settings.


#include "Tasks/Int_AbilityTask_UseSmartObjectWithGameplayBehavior.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayBehavior.h"
#include "GameplayBehaviorConfig.h"
#include "GameplayBehaviorSmartObjectBehaviorDefinition.h"
#include "GameplayBehaviorSubsystem.h"
#include "SmartObjectComponent.h"
#include "SmartObjectSubsystem.h"
#include "Behaviour/Int_GameplayBehaviour_InteractionWithAbility.h"

UInt_AbilityTask_UseSmartObjectWithGameplayBehavior::UInt_AbilityTask_UseSmartObjectWithGameplayBehavior(
const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	bBehaviorFinished = false;
}

UInt_AbilityTask_UseSmartObjectWithGameplayBehavior* UInt_AbilityTask_UseSmartObjectWithGameplayBehavior::
UseSmartObjectWithGameplayBehavior(UGameplayAbility* OwningAbility, FSmartObjectClaimHandle ClaimHandle,
	ESmartObjectClaimPriority ClaimPriority, EInteractionType InInteractionType)
{
	if (OwningAbility == nullptr) return nullptr;
	
	// Create the task
	UInt_AbilityTask_UseSmartObjectWithGameplayBehavior* MyTask = NewAbilityTask<UInt_AbilityTask_UseSmartObjectWithGameplayBehavior>(OwningAbility);
	if (MyTask == nullptr) return nullptr;
	MyTask->InteractionType = InInteractionType;
	MyTask->SetClaimHandle(ClaimHandle);
	return MyTask;
}

void UInt_AbilityTask_UseSmartObjectWithGameplayBehavior::Activate()
{
	Super::Activate();
	bool bSuccess = false;
	ON_SCOPE_EXIT
	{
		if (!bSuccess)
		{
			EndTask();
		}
	};
	if (!ensureMsgf(ClaimedHandle.IsValid(), TEXT("SmartObject handle must be valid at this point.")))
	{
		return;
	}
	APawn* Pawn = Cast<APawn>(GetAvatarActor());
	if (Pawn == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Pawn required to use GameplayBehavior"));
	}
	USmartObjectSubsystem* SO_Subsystem = USmartObjectSubsystem::GetCurrent(Pawn->GetWorld());
	if (!ensureMsgf(SO_Subsystem != nullptr, TEXT("SmartObjectSubsystem must be accessible at this point.")))
	{
		return;
	}
	// A valid claimed handle can point to an object that is no longer part of the simulation
	if (!SO_Subsystem->IsClaimedSmartObjectValid(ClaimedHandle))
	{
		UE_LOG(LogTemp, Error, TEXT("Claimed smart object is no longer valid."));
		return;
	}
	// Register a callback to be notified if the claimed slot became unavailable
	SO_Subsystem->RegisterSlotInvalidationCallback(ClaimedHandle, FOnSlotInvalidated::CreateUObject(this, &UInt_AbilityTask_UseSmartObjectWithGameplayBehavior::OnSlotInvalidated));

	bSuccess = StartInteraction();
}

void UInt_AbilityTask_UseSmartObjectWithGameplayBehavior::OnDestroy(bool bInOwnerFinished)
{
	if (ClaimedHandle.IsValid())
	{
		USmartObjectSubsystem* SmartObjectSubsystem = USmartObjectSubsystem::GetCurrent(GetWorld());
		check(SmartObjectSubsystem);
		SmartObjectSubsystem->MarkSlotAsFree(ClaimedHandle);
		SmartObjectSubsystem->UnregisterSlotInvalidationCallback(ClaimedHandle);
		ClaimedHandle.Invalidate();
	}

	if (TaskState != EGameplayTaskState::Finished)
	{
		if (GameplayBehavior != nullptr && bBehaviorFinished)
		{
			// OnSucceeded.Broadcast();
		}
		else
		{
			// OnFailed.Broadcast();
		}
	}

	Super::OnDestroy(bInOwnerFinished);
}

bool UInt_AbilityTask_UseSmartObjectWithGameplayBehavior::StartInteraction()
{
	UWorld* World = GetWorld();
	USmartObjectSubsystem* SO_Subsystem = USmartObjectSubsystem::GetCurrent(World);
	if (!ensure(SO_Subsystem)) return false;
	//Mark the slot as occupied
	const UGameplayBehaviorSmartObjectBehaviorDefinition* SmartObjectGameplayBehaviorDefinition = SO_Subsystem->MarkSlotAsOccupied<UGameplayBehaviorSmartObjectBehaviorDefinition>(ClaimedHandle);
	// Get the behavior config
	const UGameplayBehaviorConfig* GameplayBehaviorConfig = SmartObjectGameplayBehaviorDefinition != nullptr ? SmartObjectGameplayBehaviorDefinition->GameplayBehaviorConfig : nullptr;
	// Set the GameplayBehavior from the config
	GameplayBehavior = GameplayBehaviorConfig != nullptr ? GameplayBehaviorConfig->GetBehavior(*World) : nullptr;
	
	
	if (GameplayBehavior == nullptr) return false;
	UInt_GameplayBehaviour_InteractionWithAbility* InteractionBehavior = Cast<UInt_GameplayBehaviour_InteractionWithAbility>(GameplayBehavior);
	if (InteractionBehavior == nullptr) return false;
	
	InteractionBehavior->InteractionType = InteractionType;

	const USmartObjectComponent* SmartObjectComponent = SO_Subsystem->GetSmartObjectComponent(ClaimedHandle);
	AActor& InteractorActor = *GetAvatarActor();
	AActor* InteractedActor = SmartObjectComponent ? SmartObjectComponent->GetOwner() : nullptr;
	const bool bBehaviorActive = UGameplayBehaviorSubsystem::TriggerBehavior(*InteractionBehavior, InteractorActor, GameplayBehaviorConfig, InteractedActor);
	if (bBehaviorActive)
	{
		OnBehaviorFinishedNotifyHandle = GameplayBehavior->GetOnBehaviorFinishedDelegate().AddUObject(this, &UInt_AbilityTask_UseSmartObjectWithGameplayBehavior::OnSmartObjectBehaviorFinished);
	}
	return bBehaviorActive;
	
}

void UInt_AbilityTask_UseSmartObjectWithGameplayBehavior::OnSmartObjectBehaviorFinished(UGameplayBehavior& Behavior, AActor& Avatar, const bool bInterrupted)
{
	// Adding an ensure in case the assumptions change in the future.
	ensure(GetAvatarActor() != nullptr);

	// // make sure we handle the right pawn - we can get this notify for a different
	// // Avatar if the behavior sending it out is not instanced (CDO is being used to perform actions)
	if (GetAvatarActor() == &Avatar)
	{
		Behavior.GetOnBehaviorFinishedDelegate().Remove(OnBehaviorFinishedNotifyHandle);
		bBehaviorFinished = true;
		EndTask();
	}
}

void UInt_AbilityTask_UseSmartObjectWithGameplayBehavior::OnSlotInvalidated(
	const FSmartObjectClaimHandle& ClaimHandle, const ESmartObjectSlotState State)
{
	if (!bBehaviorFinished && GameplayBehavior != nullptr)
	{
		check(GetAvatarActor());
		GameplayBehavior->GetOnBehaviorFinishedDelegate().Remove(OnBehaviorFinishedNotifyHandle);
		GameplayBehavior->AbortBehavior(*GetAvatarActor());
	}
	EndTask();
}
