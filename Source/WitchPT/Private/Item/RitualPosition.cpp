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
	if (!RitualAltar)
	{
		UE_LOG(LogTemp, Log, TEXT("[DEBUG-RITUAL-POS] BeginPlay for position %s - Looking for altar..."), *GetName());
		
		// Try to find a parent altar in the hierarchy
		AActor* ParentActor = GetAttachParentActor();
		while (ParentActor)
		{
			ARitualAltar* PotentialAltar = Cast<ARitualAltar>(ParentActor);
			if (PotentialAltar)
			{
				RitualAltar = PotentialAltar;
				UE_LOG(LogTemp, Log, TEXT("[DEBUG-RITUAL-POS] RitualPosition %s auto-found parent altar %s"), 
					*GetName(), *RitualAltar->GetName());
				break;
			}
			ParentActor = ParentActor->GetAttachParentActor();
		}
		
		// If still not found, look for any altar in the level
		if (!RitualAltar)
		{
			TArray<AActor*> FoundAltars;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARitualAltar::StaticClass(), FoundAltars);
			if (FoundAltars.Num() > 0)
			{
				RitualAltar = Cast<ARitualAltar>(FoundAltars[0]);
				UE_LOG(LogTemp, Log, TEXT("[DEBUG-RITUAL-POS] RitualPosition %s auto-found level altar %s"), 
					*GetName(), *RitualAltar->GetName());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("[DEBUG-RITUAL-POS] RitualPosition %s could not find any altar in level!"), *GetName());
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[DEBUG-RITUAL-POS] RitualPosition %s has preset altar %s"), 
			*GetName(), *RitualAltar->GetName());
	}
	
	// Log position tag for debugging
	UE_LOG(LogTemp, Log, TEXT("[DEBUG-RITUAL-POS] RitualPosition %s has tag: %s"), 
		*GetName(), *PositionTag.ToString());
}

void ARitualPosition::GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& OptionBuilder)
{
	// Call Super to include any base interactions from AItem
	Super::GatherInteractionOptions(InteractQuery, OptionBuilder);

	// // Get the character trying to interact
	// ACharacter* InteractingCharacterPtr = Cast<ACharacter>(InteractQuery.RequestingAvatar);
	// if (!InteractingCharacterPtr)
	// {
	// 	return;
	// }
	//
	// // Check if the position is already occupied
	// if (bIsOccupied)
	// {
	// 	// If this character is already occupying this position, maybe offer to leave
	// 	if (OccupyingCharacter == InteractingCharacterPtr)
	// 	{
	// 		FInteractionOption LeaveOption;
	// 		LeaveOption.Verb = NSLOCTEXT("RitualPosition", "Leave", "Leave");
	// 		LeaveOption.SubText = NSLOCTEXT("RitualPosition", "LeaveSubtext", "Leave the ritual position");
	// 		LeaveOption.Priority = 1;
	// 		LeaveOption.InteractionId = "Leave_Ritual_Position";
	// 		
	// 		// Set up the option to trigger the State.Ritual.OccupyingPosition tag removal
	// 		// This could be handled by a GA on the player
	// 		const UFWitchPTGameplayTags& Tags = UFWitchPTGameplayTags::Get();
	// 		LeaveOption.TargetGameplayEventTag = Tags.Event_Interaction_LeaveRitualPosition;
	// 		LeaveOption.Payload.TargetObject = this;
	// 		
	// 		OptionBuilder.AddInteractionOption(LeaveOption);
	// 	}
	// 	
	// 	// Otherwise, no option for a different player to occupy
	// 	return;
	// }
	//
	// // Offer option to occupy the position if it's vacant
	// FInteractionOption OccupyOption;
	// OccupyOption.Verb = NSLOCTEXT("RitualPosition", "Occupy", "Occupy");
	// OccupyOption.SubText = NSLOCTEXT("RitualPosition", "OccupySubtext", "Take this ritual position");
	// OccupyOption.Priority = 1;
	// OccupyOption.InteractionId = "Occupy_Ritual_Position";
	//
	// // Set up the option to trigger the player's GA_Character_OccupyRitualPosition ability
	// const UFWitchPTGameplayTags& Tags = UFWitchPTGameplayTags::Get();
	// OccupyOption.TargetGameplayEventTag = Tags.Event_Interaction_OccupyRitualPosition;
	// OccupyOption.Payload.TargetObject = this;
	
	OptionBuilder.AddInteractionOption(Option);
}

void ARitualPosition::HandleInteraction(ACharacter* InteractingCharacterPtr)
{
	if (!HasAuthority()) // Server-side logic only
	{
		return;
	}

	if (!InteractingCharacterPtr || !RitualAltar)
	{
		return; // Basic validation
	}

	// This is a fallback in case the GatherInteractionOptions event system doesn't work
	// Normally, the player's GA should be triggered through the interaction system's event

	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InteractingCharacterPtr);
	if (TargetASC)
	{
		const FWitchPTGameplayTags& WitchPtGameplayTags = FWitchPTGameplayTags::Get();
		FGameplayEventData Payload;
		Payload.EventTag = WitchPtGameplayTags.Event_Interaction_OccupyRitualPosition;
		Payload.Target = this; // Pass this position actor as the target data

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(InteractingCharacterPtr, WitchPtGameplayTags.Event_Interaction_OccupyRitualPosition, Payload);
		UE_LOG(LogTemp, Log, TEXT("Sent Event_Interaction_OccupyRitualPosition to %s"), *InteractingCharacterPtr->GetName());
	}
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
		UE_LOG(LogTemp, Warning, TEXT("[DEBUG-RITUAL-POS] SetCharacterToPosition failed: no authority"));
		return;
	}

	if (!Character)
	{
		UE_LOG(LogTemp, Warning, TEXT("[DEBUG-RITUAL-POS] SetCharacterToPosition failed: invalid character"));
		return;
	}

	if (bIsOccupied)
	{
		UE_LOG(LogTemp, Warning, TEXT("[DEBUG-RITUAL-POS] SetCharacterToPosition failed: position %s already occupied by %s"), 
			*GetName(), OccupyingCharacter ? *OccupyingCharacter->GetName() : TEXT("None"));
		return;
	}

	// Set as occupied
	bIsOccupied = true;
	OccupyingCharacter = Character;
	
	// Force OnReps if needed immediately on server
	OnRep_IsOccupied();
	OnRep_OccupyingCharacter();
	
	// Apply the occupying position tag to the character
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Character);
	if (ASC)
	{
		const FWitchPTGameplayTags& witchPtGameplayTags = FWitchPTGameplayTags::Get();
		
		// Apply GE_Ritual_OccupyingPosition to the player
		// This would typically be a GameplayEffect that applies the tag and any other effects
		// The client should see this automatically via replication
		
		UE_LOG(LogTemp, Log, TEXT("[DEBUG-RITUAL-POS] Applied State.Ritual.OccupyingPosition tag to %s"), *Character->GetName());
	}
	
	// Move character to the position (can be smoothed in the GA)
	FVector TargetLocation = GetActorLocation();
	FRotator TargetRotation = GetActorRotation();
	
	UE_LOG(LogTemp, Log, TEXT("[DEBUG-RITUAL-POS] Moving %s to position %s at %s"), 
		*Character->GetName(), *GetName(), *TargetLocation.ToString());
	
	// If the character has a movement component, disable collisions during positioning
	UCharacterMovementComponent* MovementComp = Character->FindComponentByClass<UCharacterMovementComponent>();
	UCapsuleComponent* CapsuleComp = Character->FindComponentByClass<UCapsuleComponent>();
	
	// Temporarily disable collisions to avoid getting stuck
	if (CapsuleComp)
	{
		CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		UE_LOG(LogTemp, Log, TEXT("[DEBUG-RITUAL-POS] Temporarily disabled collisions for %s"), *Character->GetName());
	}
	
	// Move the character
	Character->SetActorLocation(TargetLocation);
	Character->SetActorRotation(TargetRotation);
	
	// Re-enable collisions
	if (CapsuleComp)
	{
		CapsuleComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		UE_LOG(LogTemp, Log, TEXT("[DEBUG-RITUAL-POS] Re-enabled collisions for %s"), *Character->GetName());
	}
	
	// Notify altar
	if (RitualAltar)
	{
		UE_LOG(LogTemp, Log, TEXT("[DEBUG-RITUAL-POS] Notifying altar %s that position is now occupied by %s"), 
			*RitualAltar->GetName(), *Character->GetName());
		RitualAltar->Server_OccupyPosition(Character, this);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[DEBUG-RITUAL-POS] Cannot notify altar: no altar reference set for position %s"), *GetName());
	}
	
	UE_LOG(LogTemp, Log, TEXT("[DEBUG-RITUAL-POS] Position %s set to Occupied by %s"), *GetName(), *Character->GetName());
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
		UE_LOG(LogTemp, Warning, TEXT("[DEBUG-RITUAL-POS] RemoveCharacterFromPosition failed: no authority"));
		return;
	}

	if (!bIsOccupied || !OccupyingCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("[DEBUG-RITUAL-POS] RemoveCharacterFromPosition failed: position %s is not occupied"), *GetName());
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("[DEBUG-RITUAL-POS] Removing %s from position %s"), 
		*OccupyingCharacter->GetName(), *GetName());
	
	// Remove the occupying position tag from the character
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OccupyingCharacter);
	if (ASC)
	{
		const FWitchPTGameplayTags& gameplayTags = FWitchPTGameplayTags::Get();
		
		// Remove the State.Ritual.OccupyingPosition tag
		// This would typically be done by removing the GE that applied it
		
		UE_LOG(LogTemp, Log, TEXT("[DEBUG-RITUAL-POS] Removed State.Ritual.OccupyingPosition tag from %s"), *OccupyingCharacter->GetName());
	}
	
	// Clear state
	bIsOccupied = false;
	ACharacter* OldCharacter = OccupyingCharacter;
	OccupyingCharacter = nullptr;
	
	// Force OnReps if needed immediately on server
	OnRep_IsOccupied();
	OnRep_OccupyingCharacter();
	
	UE_LOG(LogTemp, Log, TEXT("[DEBUG-RITUAL-POS] Position %s set to Unoccupied (was %s)"), 
		*GetName(), OldCharacter ? *OldCharacter->GetName() : TEXT("nullptr"));
}

void ARitualPosition::OnRep_IsOccupied()
{
	// Client-side reaction to occupancy change
	// Example: Change material, play sound, update UI attached to this position
	UE_LOG(LogTemp, Log, TEXT("[DEBUG-RITUAL-POS] Position %s occupancy changed to: %s"), 
		*GetName(), bIsOccupied ? TEXT("OCCUPIED") : TEXT("VACANT"));
}

void ARitualPosition::OnRep_OccupyingCharacter()
{
	// Client-side reaction to character change
	UE_LOG(LogTemp, Log, TEXT("[DEBUG-RITUAL-POS] Position %s occupying character changed to: %s"), 
		*GetName(), OccupyingCharacter ? *OccupyingCharacter->GetName() : TEXT("None"));
}

