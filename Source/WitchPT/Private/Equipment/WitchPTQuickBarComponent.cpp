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

void UWitchPTQuickBarComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (Slots.Num() < NumSlots)
	{
		Slots.AddDefaulted(NumSlots - Slots.Num());
	}
}

void UWitchPTQuickBarComponent::AddItemToSlot(int32 SlotIndex, UWitchPTInventoryItemInstance* Item)
{
	if (Slots.IsValidIndex(SlotIndex) && (Item != nullptr))
	{
		if (Slots[SlotIndex] == nullptr)
		{
			Slots[SlotIndex] = Item;
			OnRep_Slots();
		}
	}
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
	OnActiveSlotChanged.Broadcast(ActiveSlotIndex);
}

void UWitchPTQuickBarComponent::SetActiveSlotIndex_Implementation(int32 NewIndex)
{
	UE_LOG(LogTemp, Log, TEXT("Setting active slot index to %d"), NewIndex);
	if (Slots.IsValidIndex(NewIndex) && (ActiveSlotIndex != NewIndex))
	{
		UnequipItemInSlot();
		
		ActiveSlotIndex = NewIndex;
		
		EquipItemInSlot();
		
		OnRep_ActiveSlotIndex();
	}
}


