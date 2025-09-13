// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Int_InteractionSystemComponent.h"

#include "InterchangeResult.h"
#include "Int_SmartObjectFunctionLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

UInt_InteractionSystemComponent::UInt_InteractionSystemComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;


}

void UInt_InteractionSystemComponent::GetLifetimeReplicatedProps(
	TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;
	Params.Condition = COND_OwnerOnly;

	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, CurrentInteractableActor, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, NumOfInteractableActors, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, InteractingOption, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, InteractionOptions, Params);
}

UInt_InteractionSystemComponent* UInt_InteractionSystemComponent::GetInteractionSystemComponent(const AActor* Actor)
{
	return IsValid(Actor) ? Actor->FindComponentByClass<UInt_InteractionSystemComponent>() : nullptr;
}

void UInt_InteractionSystemComponent::Server_CycleInteractableActors_Implementation(bool bNext)
{
	if (bIsInteracting || InteractableActors.Num() <= 1) return;
	
	int32 Index = CurrentInteractableActor != nullptr ? InteractableActors.IndexOfByKey(CurrentInteractableActor) : 0;
	
	if (!InteractableActors.IsValidIndex(Index)) return;
	if (bNext)
	{
		Index = FMath::Clamp(Index + 1, 0, InteractableActors.Num());
	}
	else
	{
		Index = FMath::Clamp(Index - 1, 0, InteractableActors.Num());
	}
	if (InteractableActors.IsValidIndex(Index) && InteractableActors[Index] != nullptr && InteractableActors[Index] !=
		CurrentInteractableActor)
	{
		// SetCurrent(InteractableActors[Index]);
	}
}

void UInt_InteractionSystemComponent::SearchInteractableActors()
{
	//TODO: pending implementation
	// OnSearchInteractableActorsEvent.Broadcast();
}

void UInt_InteractionSystemComponent::SetCurrentInteractableActor(AActor* InActor)
{
	if (InActor != CurrentInteractableActor)
	{
		AActor* OldActor = CurrentInteractableActor;
		CurrentInteractableActor = InActor;
		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, CurrentInteractableActor, this);
		OnInteractableActorChanged(OldActor);
	}
}

void UInt_InteractionSystemComponent::SetInteractableActors(TArray<AActor*> NewActors)
{
	if (!GetOwner()->HasAuthority()) return;

	InteractableActors = NewActors;
	SetInteractableActorsNum(InteractableActors.Num()),
	OnInteractableActorsChanged();
}

void UInt_InteractionSystemComponent::SetInteractableActorsNum(int32 NewNum)
{
	if (NewNum != NumOfInteractableActors)
	{
		const int32 PrevNum = NumOfInteractableActors;
		NumOfInteractableActors = NewNum;
		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, NumOfInteractableActors, this)
		OnInteractableActorsNumChanged();
	}
}
FSmartObjectRequestFilter UInt_InteractionSystemComponent::GetSmartObjectRequestFilter_Implementation()
{
	return DefaultRequestFilter;
}

void UInt_InteractionSystemComponent::StartInteraction(int32 NewIndex)
{
	if (bIsInteracting)
	{
		UE_LOG(LogTemp, Warning, TEXT("Already interacting."));
		return;
	}
	if (!InteractionOptions.IsValidIndex(NewIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid interaction option index: %d"), NewIndex);
		return;
	}
	int32 PrevIndexInteraction = InteractingOption;
	InteractingOption = NewIndex;
	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, InteractingOption, this);
}

void UInt_InteractionSystemComponent::EndInteraction()
{
	if (!bIsInteracting) return;

	int32 PrevInteractionOption = InteractingOption;
	InteractingOption = INDEX_NONE;
	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, InteractingOption, this);
	OnInteractingOptionChanged(PrevInteractionOption);
}

void UInt_InteractionSystemComponent::InstantInteraction(int32 NewIndex)
{
	if (bIsInteracting) return;
	
	if (!InteractionOptions.IsValidIndex(NewIndex)) return;
	
	int32 Prev = InteractingOption;
	InteractingOption = NewIndex;
	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, InteractingOption, this);
	OnInteractingOptionChanged(Prev);

	int32 Prev2 = InteractingOption;
	InteractingOption = INDEX_NONE;
	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, InteractingOption, this);
	OnInteractingOptionChanged(Prev2);
}

void UInt_InteractionSystemComponent::OnInteractableActorChanged(AActor* OldActor)
{
	if (GetOwner()->GetLocalRole() >= ROLE_Authority)
	{
		RefreshOptionsForActor();
	}
}

void UInt_InteractionSystemComponent::OnInteractableActorsNumChanged()
{
}

void UInt_InteractionSystemComponent::OnInteractingOptionChanged(int32 PrevOptionIndex)
{
}
void UInt_InteractionSystemComponent::OnInteractionOptionsChanged()
{
}

void UInt_InteractionSystemComponent::RefreshOptionsForActor()
{
	USmartObjectSubsystem* Subsystem = USmartObjectSubsystem::GetCurrent(GetWorld());
	if (!Subsystem)
	{
		return;
	}
	// Getting new options for the current interacted actor.
	TArray<FInt_InteractionOption> NewOptions;
	{
		TArray<FSmartObjectRequestResult> Results;
		if (IsValid(CurrentInteractableActor) && UInt_SmartObjectFunctionLibrary::FindSmartObjectsWithInteractionEntranceInActor(GetSmartObjectRequestFilter(), CurrentInteractableActor, Results, GetOwner()))
		{
			
			UE_LOG(LogTemp, Warning, TEXT("Found %d smart object(s) with interaction entrance in actor in component %s"), Results.Num(), *CurrentInteractableActor->GetName());
			
		}
		
	}
}

void UInt_InteractionSystemComponent::OnInteractableActorsChanged_Implementation()
{
}
































