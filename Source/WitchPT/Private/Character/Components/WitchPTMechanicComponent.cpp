// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Components/WitchPTMechanicComponent.h"
#include "Item/CauldronAltar.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "Item/RitualAltar.h"
#include "Item/Components/CauldronCraftComponent.h"

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

void UWitchPTMechanicComponent::RequestUnOccupyPositionInCauldron_Implementation(ACauldronAltar* TargetCauldron)
{
	if (!TargetCauldron) return;
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (GetOwnerRole() == ROLE_Authority)
	{
		TargetCauldron->UnoccupyPosition(Character, nullptr);
	}
}

void UWitchPTMechanicComponent::RequestSetIngredientInSlot_Implementation(ACauldronAltar* TargetAltar,
	TSubclassOf<UWitchPTInventoryItemDefinition> IngredientItemDef)
{
	if (!TargetAltar) return;
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	
	
	if(GetOwnerRole() == ROLE_Authority)
	{
		TargetAltar->TrySetIngredientInSlot(Character, IngredientItemDef);
	}
	else
	{
		Server_RequestSetIngredientInSlot(TargetAltar, IngredientItemDef);
	}

	
	
}

void UWitchPTMechanicComponent::RequestPrintDebugData_Implementation(ACauldronAltar* TargetAltar)
{
	if (!TargetAltar) return;
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	
	if (GetOwnerRole() == ROLE_Authority)
	{
		FVector SpawnLocation = TargetAltar->GetActorLocation();
		FPotionResult Result = TargetAltar->CauldronCraftComponent->CraftPotion(false, nullptr, SpawnLocation);
	}
}

void UWitchPTMechanicComponent::RequestCraftPotion_Implementation(ACauldronAltar* TargetAltar)
{
	if (!TargetAltar) return;
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (GetOwnerRole() == ROLE_Authority)
	{
		TargetAltar->CauldronCraftComponent->CraftPotion(false, Character, TargetAltar->GetActorLocation());
	}
	else
	{
		Server_RequestCraftPotion(TargetAltar);
	}
	
}

void UWitchPTMechanicComponent::Server_RequestSetIngredientInSlot_Implementation(ACauldronAltar* TargetAltar,
                                                                                 TSubclassOf<UWitchPTInventoryItemDefinition> IngredientItemDef)
{
	if (!TargetAltar) return;
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	
	TargetAltar->TrySetIngredientInSlot(Character, IngredientItemDef);
	
}

void UWitchPTMechanicComponent::RequestOccupyPosition_Implementation(ARitualAltar* TargetAltar, ABaseInteractionPosition* Position)
{
	
	if (!TargetAltar) return;
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	
	if (GetOwnerRole() == ROLE_Authority)
	{
		TargetAltar->OccupyPosition(Character, Position);
	}
	
}

void UWitchPTMechanicComponent::RequestUnOccupyPositionInRitual_Implementation(ARitualAltar* TargetAltar,
	ABaseInteractionPosition* Position)
{
	if (!TargetAltar) return;
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (GetOwnerRole() == ROLE_Authority)
	{
		TargetAltar->UnoccupyPosition(Character, Position);
	}
	
}

void UWitchPTMechanicComponent::Server_RequestCraftPotion_Implementation(ACauldronAltar* TargetAltar)
{
	if (!TargetAltar) return;
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	TargetAltar->CauldronCraftComponent->CraftPotion(false, Character, TargetAltar->GetActorLocation());
}

void UWitchPTMechanicComponent::RequestStartRitual_Implementation(ARitualAltar* TargetAltar)
{
	if (!TargetAltar) return;
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (GetOwnerRole() == ROLE_Authority)
	{
		TargetAltar->StartRitual(Character);
	}
}

void UWitchPTMechanicComponent::RequestRitualInput_Implementation(ARitualAltar* TargetAltar, const FGameplayTag& InputTag)
{
	if (!TargetAltar) return;
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (GetOwnerRole() == ROLE_Authority)
	{
		TargetAltar->HandlePlayerInput(Character, InputTag);
	}
}



