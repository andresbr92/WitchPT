// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Int_InteractionSystemComponent.h"

UInt_InteractionSystemComponent::UInt_InteractionSystemComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;


}

void UInt_InteractionSystemComponent::GetLifetimeReplicatedProps(
	TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

UInt_InteractionSystemComponent* UInt_InteractionSystemComponent::GetInteractionSystemComponent(const AActor* Actor)
{
	return IsValid(Actor) ? Actor->FindComponentByClass<UInt_InteractionSystemComponent>() : nullptr;
}

void UInt_InteractionSystemComponent::Server_CycleInteractableActors_Implementation(bool bNext)
{
}

void UInt_InteractionSystemComponent::OnInteractableActorChanged(AActor* OldActor)
{
}

void UInt_InteractionSystemComponent::OnInteractableActorsNumChanged()
{
}

void UInt_InteractionSystemComponent::OnInteractingOptionChanged(int32 PrevOptionIndex)
{
}

