// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Components/WitchPTMechanicComponent.h"
#include "Item/CauldronAltar.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "Item/RitualAltar.h"

// Sets default values for this component's properties
UWitchPTMechanicComponent::UWitchPTMechanicComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UWitchPTMechanicComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	// Agregar propiedades replicadas aquí si es necesario
}

// Called when the game starts
void UWitchPTMechanicComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void UWitchPTMechanicComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// --- RITUAL IMPLEMENTATION ---
void UWitchPTMechanicComponent::RequestStartBrewingPotion_Implementation(ACauldronAltar* TargetAltar)
{
	if (!TargetAltar)
	{
		return;
	}

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character)
	{
		return;
	}
	
	if (Character->HasAuthority())
	{
		TargetAltar->StartBrewingPotion(Character);
	}
}

void UWitchPTMechanicComponent::RequestStartCarryCauldron_Implementation(ACauldronAltar* TargetAltar)
{
	if (!TargetAltar)
	{
		return;
	}

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character)
	{
		return;
	}
	
	if (GetOwnerRole() == ROLE_Authority)
	{
		// Si estamos en el servidor, llamamos directamente
		TargetAltar->StartCarryCauldron(Character);
	}
	
}

void UWitchPTMechanicComponent::RequestStartPlacementPreview_Implementation(ACauldronAltar* TargetAltar)
{
	if (!TargetAltar)
	{
		return;
	}

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character)
	{
		return;
	}
	
	if (GetOwnerRole() == ROLE_Authority)
	{
		// Si estamos en el servidor, llamamos directamente
		TargetAltar->StartPlacementPreview(Character);
	}
}

void UWitchPTMechanicComponent::RequestUpdatePlacementPreview_Implementation(ACauldronAltar* TargetAltar, const FVector& HitLocation, const FVector& HitNormal)
{
	if (!TargetAltar)
	{
		return;
	}
	
	if (GetOwnerRole() == ROLE_Authority)
	{
		// Si estamos en el servidor, llamamos directamente
		TargetAltar->UpdatePlacementPreview(HitLocation, HitNormal);
	}
}

void UWitchPTMechanicComponent::RequestCancelPlacementPreview_Implementation(ACauldronAltar* TargetAltar)
{
	if (!TargetAltar)
	{
		return;
	}
	
	if (GetOwnerRole() == ROLE_Authority)
	{
		// Si estamos en el servidor, llamamos directamente
		TargetAltar->CancelPlacement();
	}
	
}

void UWitchPTMechanicComponent::RequestFinalizePlacement_Implementation(ACauldronAltar* TargetAltar)
{
	if (!TargetAltar)
	{
		return;
	}
	
	if (GetOwnerRole() == ROLE_Authority)
	{
		// Si estamos en el servidor, llamamos directamente
		TargetAltar->FinalizePlacement();
	}

}

void UWitchPTMechanicComponent::RequestOccupyPosition_Implementation(ARitualAltar* TargetAltar,
	ABaseInteractionPosition* Position)
{
	if (!TargetAltar) return;
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (GetOwnerRole() == ROLE_Authority)
	{
		TargetAltar->OccupyPosition(Character, Position);
	}
	
}



