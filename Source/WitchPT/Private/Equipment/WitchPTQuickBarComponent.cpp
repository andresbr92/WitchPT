// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/WitchPTQuickBarComponent.h"

#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UWitchPTQuickBarComponent::UWitchPTQuickBarComponent()
{
	SetIsReplicatedByDefault(true);
}

void UWitchPTQuickBarComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, Slots);
	DOREPLIFETIME(ThisClass, ActiveSlotIndex);
}

void UWitchPTQuickBarComponent::EquipItemInSlot()
{
}

void UWitchPTQuickBarComponent::UnequipItemInSlot()
{
}

void UWitchPTQuickBarComponent::OnRep_Slots()
{
}

void UWitchPTQuickBarComponent::OnRep_ActiveSlotIndex()
{
}

void UWitchPTQuickBarComponent::SetActiveSlotIndex_Implementation(int32 NewIndex)
{
}


