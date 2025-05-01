// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/RitualPosition.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "Item/RitualAltar.h" // Include the Altar header
#include "GameplayTagContainer.h" // For tags

// Sets default values
ARitualPosition::ARitualPosition()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false; // Position likely doesn't need to tick

	bReplicates = true;
	

	

	

	// Default values for replicated properties
	bIsOccupied = false;
	OccupyingCharacter = nullptr;
	RitualAltar = nullptr; // Should be set in the editor or found in BeginPlay
}

void ARitualPosition::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARitualPosition, bIsOccupied);
	DOREPLIFETIME(ARitualPosition, OccupyingCharacter);
	// RitualAltar and PositionTag are usually set once and don't need replication unless they can change dynamically
}

void ARitualPosition::BeginPlay()
{
	Super::BeginPlay();

	// Optional: If RitualAltar isn't set via EditInstanceOnly, you might try finding it here
	// Example: if (!RitualAltar) { RitualAltar = Cast<ARitualAltar>(GetOwner()); }
	// Ensure RitualAltar is valid before proceeding with logic that depends on it.
}

void ARitualPosition::GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& OptionBuilder)
{
	// Call Super if AItem has base interaction logic you want to preserve
	// Super::GatherInteractionOptions(InteractQuery, OptionBuilder);

	

	// ACharacter* InteractingCharacterPtr = Cast<ACharacter>(InteractQuery.RequestingAvatar);
	// if (!InteractingCharacterPtr)
	// {
	// 	return;
	// }

	// Check if the position is already occupied
	

	// Check if the ritual is in a state that allows occupying (e.g., Idle or Preparing)
	// This requires reading the Altar's state attribute - might be complex here.
	// For now, assume interaction is possible if unoccupied.

	// Offer the option to occupy the position
	
	
	
	// Option.TargetAbilitySystem = InteractQuery.RequestingASC.Get(); // Target the player's ASC
	// Option.SourceAbilitySystem = nullptr; // Interaction source is the position itself

	// According to the design, the PLAYER triggers a GA.
	// We can either:
	// 1. Grant a temporary ability here (complex).
	// 2. Send a Gameplay Event to the player's ASC, which triggers their GA_Player_OccupyRitualPosition. (Preferred)
	// 3. Have the interaction system directly call a function on the player (less GAS-idiomatic).

	// Let's assume Option 2: Send a Gameplay Event.
	// The interaction system needs to be set up to handle sending events based on the chosen option.
	// We'll define the event tag here. The player's GA must be configured to trigger from this tag.
	//Option.TargetGameplayEventTag = WitchPTGameplayTags::Event_Interaction_OccupyRitualPosition; // Use your actual tag
	//Option.Payload.TargetObject = this; // Pass this position as the target object in the event payload

	OptionBuilder.AddInteractionOption(Option);
}





// This function is likely called by the interaction system when the "OccupyPosition" option is chosen.
// It might need adjustment based on how your interaction system executes options.
// Alternatively, the logic could be directly within the player's GA_Player_OccupyRitualPosition.
void ARitualPosition::HandleInteraction(ACharacter* InteractingCharacterPtr)
{
	if (!HasAuthority()) // Server-side logic only
	{
		return;
	}

	if (!InteractingCharacterPtr || !RitualAltar || bIsOccupied)
	{
		return; // Basic validation
	}

	// The design states the PLAYER'S GA calls the Altar's RPC.
	// Therefore, this function might not be strictly necessary if GatherInteractionOptions
	// correctly sets up the interaction system to send the Gameplay Event to the player.
	// If the interaction system *doesn't* send the event, you'd do it here:
	/*
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InteractingCharacterPtr);
	if (TargetASC)
	{
		FGameplayEventData Payload;
		Payload.EventTag = WitchPTGameplayTags::Event_Interaction_OccupyRitualPosition;
		Payload.Target = this; // Pass this position actor as the target data

		TargetASC->HandleGameplayEvent(Payload.EventTag, &Payload);
		UE_LOG(LogTemp, Log, TEXT("Sent Event_Interaction_OccupyRitualPosition to %s"), *InteractingCharacterPtr->GetName());
	}
	*/

	// For now, assume GatherInteractionOptions + Interaction System handles sending the event.
	// UE_LOG(LogTemp, Log, TEXT("HandleInteraction called for %s on Position %s. Event should be sent by Interaction System."),
	// 	*InteractingCharacterPtr->GetName(), *GetName());

}

void ARitualPosition::SetPositionTag(const FGameplayTag& NewTag)
{
	PositionTag = NewTag;
}


// Called on the Server by the Altar (or potentially a GA)
void ARitualPosition::SetCharacterToPosition(ACharacter* Character)
{
	if (!HasAuthority())
	{
		return;
	}

	if (!bIsOccupied && Character)
	{
		bIsOccupied = true;
		OccupyingCharacter = Character;
		// Force OnReps if needed immediately on server
		OnRep_IsOccupied();
		OnRep_OccupyingCharacter();
		OccupyingCharacter = Character; // Set the character occupying this position
		UE_LOG(LogTemp, Log, TEXT("Position %s set to Occupied by %s"), *GetName(), *Character->GetName());
		RitualAltar->Server_OccupyPosition(Character, this);
	}
}

void ARitualPosition::SetUnoccupied_Implementation()
{
	RemoveCharacterFromPosition();
}

void ARitualPosition::SetOccupied_Implementation(ACharacter* Character)
{
	SetCharacterToPosition(Character);
}

// Called on the Server by the Altar (or potentially a GA)
void ARitualPosition::RemoveCharacterFromPosition()
{
	if (!HasAuthority())
	{
		return;
	}

	if (bIsOccupied)
	{
		bIsOccupied = false;
		ACharacter* OldCharacter = OccupyingCharacter;
		OccupyingCharacter = nullptr;
		// Force OnReps if needed immediately on server
		OnRep_IsOccupied();
		OnRep_OccupyingCharacter();
		// UE_LOG(LogTemp, Log, TEXT("Position %s set to Unoccupied (was %s)"), *GetName(), OldCharacter ? *OldCharacter->GetName() : TEXT("nullptr"));
	}
}

void ARitualPosition::OnRep_IsOccupied()
{
	// Client-side reaction to occupancy change
	// Example: Change material, play sound, update UI attached to this position
	// UE_LOG(LogTemp, Log, TEXT("Client %d: Position %s OnRep_IsOccupied: %s"), GPlayInEditorID, *GetName(), bIsOccupied ? TEXT("true") : TEXT("false"));
}

void ARitualPosition::OnRep_OccupyingCharacter()
{
	// Client-side reaction to character change
	// UE_LOG(LogTemp, Log, TEXT("Client %d: Position %s OnRep_OccupyingCharacter: %s"), GPlayInEditorID, *GetName(), OccupyingCharacter ? *OccupyingCharacter->GetName() : TEXT("nullptr"));
}
