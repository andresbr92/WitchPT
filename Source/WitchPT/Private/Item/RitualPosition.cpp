// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/RitualPosition.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "Item/RitualAltar.h" // Include the Altar header
#include "GameplayTagContainer.h" // For tags
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "FWitchPTGameplayTags.h"
#include "GameplayEffect.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARitualPosition::ARitualPosition()
{
	// No need to set up replication here - it's handled by the base class
}

void ARitualPosition::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ARitualPosition, RitualAltar);
}

// void ARitualPosition::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
// {
// 	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//
// 	DOREPLIFETIME(ARitualPosition, bIsOccupied);
// 	DOREPLIFETIME(ARitualPosition, OccupyingCharacter);
// 	// RitualAltar and PositionTag are usually set once and don't need replication unless they can change dynamically
// }

void ARitualPosition::BeginPlay()
{
	Super::BeginPlay();
	
	// Try to find the RitualAltar if it wasn't set in the editor
	
}


void ARitualPosition::GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& OptionBuilder)
{
	Super::GatherInteractionOptions(InteractQuery, OptionBuilder);
	
	// Ritual-specific interaction options
	
	// ACharacter* InteractingCharacterPtr = Cast<ACharacter>(InteractQuery.User);
	//
	// // If no player is occupying the position, let a player take the position
	// if (!IsOccupied() && InteractingCharacterPtr)
	// {
	// 	OptionBuilder.AddDefaultInteraction(
	// 		FText::FromString("Take Position"), // Text for the interaction
	// 		[this, InteractingCharacterPtr](const FInteractionExecuteContext& Context)
	// 		{
	// 			HandleInteraction(InteractingCharacterPtr);
	// 		}
	// 	);
	// }
	//
	// // Special case: if the character is standing at a position and we're in Inactive state, they can initiate the ritual
	// if (IsOccupied() && RitualAltar && GetOccupyingCharacter() == InteractingCharacterPtr)
	// {
	// 	// Check ritual state
	// 	if (RitualAltar->GetCurrentState() == EInteractionState::Inactive)
	// 	{
	// 		OptionBuilder.AddDefaultInteraction(
	// 			FText::FromString("Start Ritual"), // Text for the interaction
	// 			[this, InteractingCharacterPtr](const FInteractionExecuteContext& Context)
	// 			{
	// 				if (RitualAltar)
	// 				{
	// 					// Cast to the appropriate start ritual function
	// 					RitualAltar->StartRitual(InteractingCharacterPtr);
	// 				}
	// 			}
	// 		);
	// 	}
	// }
}

ARitualAltar* ARitualPosition::GetRitualAltarActor_Implementation() const
{
	if (RitualAltar)
	{
		return RitualAltar;
	}
	return nullptr;
}

void ARitualPosition::SendStartRitualRequest_Implementation(ACharacter* RequestingCharacter)
{
	if (RitualAltar && RequestingCharacter && HasAuthority())
	{
		RitualAltar->Server_StartRitual(RequestingCharacter);
	}
}

void ARitualPosition::SendPlayerInput_Implementation(ACharacter* InputCharacter, const FGameplayTag& InputTag)
{
	if (RitualAltar && InputCharacter && InputTag.IsValid() && HasAuthority())
	{
		RitualAltar->Server_HandlePlayerInput(InputCharacter, InputTag);
	}
}

void ARitualPosition::SendPlayerOccupiedPosition_Implementation(ACharacter* InputCharacter)
{
	if (RitualAltar && InputCharacter && HasAuthority())
	{
		RitualAltar->Server_OccupyPosition(InputCharacter, this);
	}
}


void ARitualPosition::HandleInteraction(ACharacter* InteractingCharacter)
{
	Super::HandleInteraction(InteractingCharacter);
	
	if (!InteractingCharacter || !RitualAltar)
	{
		return;
	}
	
	// Call the ritual altar to handle occupation
	// RitualAltar->OccupyPosition(InteractingCharacter, this);
}





// void ARitualPosition::SetOccupied(ACharacter* Character)
// {
// 	if (!HasAuthority())
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("[DEBUG-RITUAL-POS] SetCharacterToPosition failed: no authority"));
// 		return;
// 	}
//
// 	if (!Character)
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("[DEBUG-RITUAL-POS] SetCharacterToPosition failed: invalid character"));
// 		return;
// 	}
//
// 	if (bIsOccupied)
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("[DEBUG-RITUAL-POS] SetCharacterToPosition failed: position %s already occupied by %s"), 
// 			*GetName(), OccupyingCharacter ? *OccupyingCharacter->GetName() : TEXT("None"));
// 		return;
// 	}
//
// 	// Set as occupied
// 	bIsOccupied = true;
// 	OccupyingCharacter = Character;
// 	
// 	// Force OnReps if needed immediately on server
// 	OnRep_IsOccupied();
// 	OnRep_OccupyingCharacter();
// 	
// 	UE_LOG(LogTemp, Log, TEXT("[DEBUG-RITUAL-POS] Position %s set to Occupied by %s"), *GetName(), *Character->GetName());
// }

// Called on the Server by the Altar (or potentially a GA)
// void ARitualPosition::RemoveCharacterFromPosition()
// {
// 	if (!HasAuthority())
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("[DEBUG-RITUAL-POS] RemoveCharacterFromPosition failed: no authority"));
// 		return;
// 	}
//
// 	if (!bIsOccupied || !OccupyingCharacter)
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("[DEBUG-RITUAL-POS] RemoveCharacterFromPosition failed: position %s is not occupied"), *GetName());
// 		return;
// 	}
//
// 	UE_LOG(LogTemp, Log, TEXT("[DEBUG-RITUAL-POS] Removing %s from position %s"), 
// 		*OccupyingCharacter->GetName(), *GetName());
// 	
// 	// Remove the occupying position tag from the character
// 	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OccupyingCharacter);
// 	if (ASC)
// 	{
// 		const FWitchPTGameplayTags& gameplayTags = FWitchPTGameplayTags::Get();
// 		
// 		// Remove the State.Ritual.OccupyingPosition tag
// 		// This would typically be done by removing the GE that applied it
// 		
// 		UE_LOG(LogTemp, Log, TEXT("[DEBUG-RITUAL-POS] Removed State.Ritual.OccupyingPosition tag from %s"), *OccupyingCharacter->GetName());
// 	}
// 	
// 	// Clear state
// 	bIsOccupied = false;
// 	ACharacter* OldCharacter = OccupyingCharacter;
// 	OccupyingCharacter = nullptr;
// 	
// 	// Force OnReps if needed immediately on server
// 	OnRep_IsOccupied();
// 	OnRep_OccupyingCharacter();
// 	
// 	UE_LOG(LogTemp, Log, TEXT("[DEBUG-RITUAL-POS] Position %s set to Unoccupied (was %s)"), 
// 		*GetName(), OldCharacter ? *OldCharacter->GetName() : TEXT("nullptr"));
// }
//
// void ARitualPosition::OnRep_IsOccupied()
// {
// 	// Client-side reaction to occupancy change
// 	// Example: Change material, play sound, update UI attached to this position
// 	UE_LOG(LogTemp, Log, TEXT("[DEBUG-RITUAL-POS] Position %s occupancy changed to: %s"), 
// 		*GetName(), bIsOccupied ? TEXT("OCCUPIED") : TEXT("VACANT"));
// }
//
// void ARitualPosition::OnRep_OccupyingCharacter()
// {
// 	// Client-side reaction to character change
// 	UE_LOG(LogTemp, Log, TEXT("[DEBUG-RITUAL-POS] Position %s occupying character changed to: %s"), 
// 		*GetName(), OccupyingCharacter ? *OccupyingCharacter->GetName() : TEXT("None"));
// }

