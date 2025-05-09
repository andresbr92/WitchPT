// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Components/WitchPTMechanicComponent.h"

#include "GameFramework/Character.h"

// Sets default values for this component's properties
UWitchPTMechanicComponent::UWitchPTMechanicComponent()
{

	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);


}

void UWitchPTMechanicComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

ARitualAltar* UWitchPTMechanicComponent::GetRitualAltarActor_Implementation() const
{
	return IMechanicsInterface::GetRitualAltarActor_Implementation();
}

void UWitchPTMechanicComponent::SendStartRitualRequest_Implementation(ACharacter* RequestingCharacter)
{
	IMechanicsInterface::SendStartRitualRequest_Implementation(RequestingCharacter);
}

void UWitchPTMechanicComponent::SendPlayerInput_Implementation(ACharacter* InputCharacter, const FGameplayTag& InputTag)
{
	IMechanicsInterface::SendPlayerInput_Implementation(InputCharacter, InputTag);
}

void UWitchPTMechanicComponent::SendPlayerOccupiedPosition_Implementation(ACharacter* InputCharacter)
{
	IMechanicsInterface::SendPlayerOccupiedPosition_Implementation(InputCharacter);
}

bool UWitchPTMechanicComponent::IsOccuppied_Implementation() const
{
	return IMechanicsInterface::IsOccuppied_Implementation();
}

void UWitchPTMechanicComponent::SendStartBrewingPotionRequest_Implementation(ACharacter* RequestingCharacter)
{
	if (GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("RequestingCharacter has authority"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("RequestingCharacter does not have authority"));
	}
}

void UWitchPTMechanicComponent::SendStartCarryCauldronRequest_Implementation(ACharacter* RequestingCharacter)
{
	IMechanicsInterface::SendStartCarryCauldronRequest_Implementation(RequestingCharacter);
}

void UWitchPTMechanicComponent::SendStartPlacementPreview_Implementation(ACharacter* RequestingCharacter)
{
	IMechanicsInterface::SendStartPlacementPreview_Implementation(RequestingCharacter);
}

void UWitchPTMechanicComponent::SendUpdatePlacementPreview_Implementation(const FVector& HitLocation,
	const FVector& HitNormal)
{
	IMechanicsInterface::SendUpdatePlacementPreview_Implementation(HitLocation, HitNormal);
}

void UWitchPTMechanicComponent::SendCancelPlacementPreview_Implementation()
{
	IMechanicsInterface::SendCancelPlacementPreview_Implementation();
}

void UWitchPTMechanicComponent::SendFinalizePlacement_Implementation()
{
	IMechanicsInterface::SendFinalizePlacement_Implementation();
}


void UWitchPTMechanicComponent::BeginPlay()
{
	Super::BeginPlay();

	
	
}



void UWitchPTMechanicComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}

