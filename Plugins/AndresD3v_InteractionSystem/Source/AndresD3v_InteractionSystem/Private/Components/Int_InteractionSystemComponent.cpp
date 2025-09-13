// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Int_InteractionSystemComponent.h"

#include "InterchangeResult.h"
#include "Int_InteractableInterface.h"
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

void UInt_InteractionSystemComponent::OnInteractingOptionChanged_Implementation(int32 PrevOptionIndex)
{
	bool bPrevInteracting = bIsInteracting;
	bIsInteracting = InteractingOption != INDEX_NONE;
	
	if (IsValid(CurrentInteractableActor) && CurrentInteractableActor->GetClass()->ImplementsInterface(UInt_InteractableInterface::StaticClass()))
	{
		if (!bPrevInteracting && bIsInteracting)
		{
			IInt_InteractableInterface::Execute_OnInteractionStarted(CurrentInteractableActor, GetOwner(), InteractingOption);
		}
		if (bPrevInteracting && !bIsInteracting)
		{
			IInt_InteractableInterface::Execute_OnInteractionEnded(CurrentInteractableActor, GetOwner(), InteractingOption);
		}
	}
	// TODO: implement event broadcast interacting
}


void UInt_InteractionSystemComponent::OnInteractionOptionsChanged()
{
	// TODO: implement event broadcast
}

void UInt_InteractionSystemComponent::RefreshOptionsForActor()
{
	USmartObjectSubsystem* SO_Subsystem = USmartObjectSubsystem::GetCurrent(GetWorld());
	if (!SO_Subsystem)
	{
		return;
	}
	// Getting new options for the current interacted actor.
	TArray<FInt_InteractionOption> NewOptions;
	{
		TArray<FSmartObjectRequestResult> Results;
		if (IsValid(CurrentInteractableActor) && UInt_SmartObjectFunctionLibrary::FindSmartObjectsWithInteractionEntranceInActor(GetSmartObjectRequestFilter(), CurrentInteractableActor, Results, GetOwner()))
		{
			
			for (int32 i = 0; i < Results.Num(); i++)
			{
				FInt_InteractionOption Option;
				UInt_InteractionDefinition* FoundDefinition;
				if (UInt_SmartObjectFunctionLibrary::FindInteractionDefinitionFromSmartObjectSlot(this, Results[i].SlotHandle, FoundDefinition))
				{
					Option.Definition = FoundDefinition;
					Option.RequestResult = Results[i];
					Option.SlotState = SO_Subsystem->GetSlotState(Results[i].SlotHandle);
					Option.SlotIndex = i;
					Option.BehaviorDefinition = SO_Subsystem->GetBehaviorDefinitionByRequestResult(Results[i], USmartObjectBehaviorDefinition::StaticClass());
					NewOptions.Add(Option);
				}
			}
		}
	}
	// check any options changed.
	bool bOptionsChanged = false;
	{
		if (NewOptions.Num() == InteractionOptions.Num())
		{
			NewOptions.Sort();

			for (int OptionIndex = 0; OptionIndex < NewOptions.Num(); OptionIndex++)
			{
				const FInt_InteractionOption& NewOption = NewOptions[OptionIndex];
				const FInt_InteractionOption& CurrentOption = InteractionOptions[OptionIndex];

				if (NewOption != CurrentOption)
				{
					bOptionsChanged = true;
					break;
				}
			}
		}
		else
		{
			bOptionsChanged = true;
		}
	}
	if (bOptionsChanged)
	{
		// TODO: Unregister callbacks.
		
		InteractionOptions = NewOptions;
		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, InteractionOptions, this);
		UE_LOG(LogTemp, Warning, TEXT("Interaction options changed, new count: %d"), InteractionOptions.Num());
		
		OnInteractionOptionsChanged();
		
	}
}

void UInt_InteractionSystemComponent::OnInteractableActorsChanged_Implementation()
{
}
































