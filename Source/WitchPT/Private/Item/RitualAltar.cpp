// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/RitualAltar.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet/RitualAttributeSet.h" // Assuming path, adjust if needed
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "Item/RitualPosition.h" // Include if needed for type safety
#include "Components/BoxComponent.h" // Added include
#include "AbilitySystemBlueprintLibrary.h" // For TryActivateAbilityByClass
#include "FWitchPTGameplayTags.h"
#include "GameplayAbilitySpec.h" // For checking ability grants
// #include "Npc/LevitatedNpc.h" // TODO: Replace with your actual NPC class header

// Sets default values
ARitualAltar::ARitualAltar()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true; // Keep Tick for now, might be needed for timers later

	bReplicates = true;
	

	// Create Ability System Component
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal); // Or Mixed/Full depending on needs

	// Create Attribute Set
	AttributeSet = CreateDefaultSubobject<URitualAttributeSet>(TEXT("AttributeSet"));

	// Create Deposit Volume
	DepositVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("DepositVolume"));
	DepositVolume->SetupAttachment(RootComponent); // Attach to root or another component
	DepositVolume->SetCollisionProfileName(TEXT("OverlapAllDynamic")); // Or a custom profile
	DepositVolume->SetGenerateOverlapEvents(true);
	// Adjust size and position in Blueprint or here
	DepositVolume->SetBoxExtent(FVector(100.f, 100.f, 50.f));
}

void ARitualAltar::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARitualAltar, CurrentSequence);
	DOREPLIFETIME(ARitualAltar, ParticipatingPlayers);
	DOREPLIFETIME(ARitualAltar, CurrentActivePlayer);
	DOREPLIFETIME(ARitualAltar, RitualPositions);
	// Note: Attributes in the AttributeSet handle their own replication via the ASC
}

// Called when the game starts or when spawned
void ARitualAltar::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent)
	{
		// Initialize the Ability System Component with its owner and avatar actor
		AbilitySystemComponent->InitAbilityActorInfo(this, this);

		// Grant any startup abilities or apply passive effects here if needed
		// Example: GrantAbilities(); ApplyPassiveEffects();

		// Grant the GAs defined in the header (if not granted elsewhere)
		// This ensures the Altar *has* the abilities to activate later.
		// Do this only on the server.
		if (HasAuthority())
		{
			if (GA_Ritual_Start_Class) AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(GA_Ritual_Start_Class, 1, INDEX_NONE, this));
			if (GA_Ritual_AdvanceTurn_Class) AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(GA_Ritual_AdvanceTurn_Class, 1, INDEX_NONE, this));
			if (GA_Ritual_FailInput_Class) AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(GA_Ritual_FailInput_Class, 1, INDEX_NONE, this));
			if (GA_Ritual_Succeed_Class) AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(GA_Ritual_Succeed_Class, 1, INDEX_NONE, this));
			if (GA_Ritual_CatastrophicFail_Class) AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(GA_Ritual_CatastrophicFail_Class, 1, INDEX_NONE, this));
			// Grant GE application abilities if needed
		}
	}

	// Bind overlap delegate (only on server to prevent redundant checks/activations)
	if (HasAuthority() && DepositVolume)
	{
		DepositVolume->OnComponentBeginOverlap.AddDynamic(this, &ARitualAltar::OnDepositVolumeOverlapBegin);
	}
}

// Called every frame
void ARitualAltar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

UAbilitySystemComponent* ARitualAltar::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ARitualAltar::Server_OccupyPosition_Implementation(ACharacter* Player, ARitualPosition* Position)
{
	// --- Logic for Server_OccupyPosition will be added later ---
	// Basic validation:
	if (!Player || !Position)
	{
		return;
	}
	ParticipatingPlayers.Add(Player); // Add player to the list of participating players

	// Check if position is already occupied (logic might be in ARitualPosition or here)
	// Check if player is already participating

	// Add player to ParticipatingPlayers if valid
	// Set position as occupied
	// Potentially update CurrentActivePlayer if this is the first player

	UE_LOG(LogTemp, Log, TEXT("Server_OccupyPosition called by %s for Position %s"), *Player->GetName(), *Position->GetName());
}

void ARitualAltar::OnRep_ParticipatingPlayers()
{
	// Optional: Add logic here if the client needs to react specifically
	// when the list of participating players changes.
	// For example, updating UI elements that show player portraits.
	UE_LOG(LogTemp, Log, TEXT("OnRep_ParticipatingPlayers: Count = %d"), ParticipatingPlayers.Num());
}


void ARitualAltar::OnDepositVolumeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority() || !OtherActor || OtherActor == this)
	{
		return; // Only run on server, ignore self-overlap
	}

	// TODO: Replace 'ALevitatedNpc' with the actual class of your deposited NPC
	// ALevitatedNpc* DepositedNpc = Cast<ALevitatedNpc>(OtherActor);
	// if (!DepositedNpc)
	// {
	//	 return; // Not the correct type of actor
	// }

	UE_LOG(LogTemp, Warning, TEXT("ARitualAltar: DepositVolume overlapped by %s. Checking conditions to start ritual."), *OtherActor->GetName());

	// --- Condition Checks ---

	// 1. Check if the ritual is already active or in another non-startable state
	if (AbilitySystemComponent && AttributeSet)
	{
		ERitualState CurrentState = static_cast<ERitualState>(AttributeSet->GetRitualState());
		if (CurrentState != ERitualState::Idle && CurrentState != ERitualState::Preparing)
		{
			UE_LOG(LogTemp, Warning, TEXT("Ritual cannot start: Current state is %d"), static_cast<int32>(CurrentState));
			return; // Already running or finished
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Ritual cannot start: ASC or AttributeSet is null."));
		return;
	}


	// 2. Check if there are enough participating players (at least 1)
	// We need to ensure ParticipatingPlayers is up-to-date. The Server_OccupyPosition RPC should handle adding players.
	// Let's refine the check to look for players with the correct tag occupying positions.
	int32 PlayersInPositionCount = 0;
	ParticipatingPlayers.Empty(); // Clear just before recount
	for (ARitualPosition* Position : RitualPositions) // Assuming RitualPositions array is populated
	{
		if (Position && Position->IsOccupied())
		{
			ACharacter* Occupant = Position->GetOccupyingCharacter();
			if (Occupant)
			{
				UAbilitySystemComponent* PlayerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Occupant);
				// Check if the player has the required tag (State.Ritual.OccupyingPosition)
				if (PlayerASC && PlayerASC->HasMatchingGameplayTag(FWitchPTGameplayTags::Get().State_Ritual_OccupyingPosition))
				{
					PlayersInPositionCount++;
					ParticipatingPlayers.AddUnique(Occupant); // Add to the list for this ritual attempt
				}
				else
				{
					// Log if a player is in position but lacks the tag (potential state mismatch)
					UE_LOG(LogTemp, Warning, TEXT("Player %s in position %s but lacks State.Ritual.OccupyingPosition tag."), *Occupant->GetName(), *Position->GetName());
				}
			}
		}
	}


	if (PlayersInPositionCount == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ritual cannot start: No players currently occupying positions with the correct tag."));
		// Optionally provide feedback to the player who deposited the NPC
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Ritual start conditions met (%d players in position). Attempting to activate GA_Ritual_Start."), PlayersInPositionCount);

	// --- Activate Start Ability ---
	if (AbilitySystemComponent && GA_Ritual_Start_Class)
	{
		bool bActivated = AbilitySystemComponent->TryActivateAbilityByClass(GA_Ritual_Start_Class);
		if (bActivated)
		{
			UE_LOG(LogTemp, Log, TEXT("Successfully activated GA_Ritual_Start."));
			// Optionally destroy the deposited NPC actor now that the ritual has started
			// DepositedNpc->Destroy();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to activate GA_Ritual_Start. Check ability conditions/costs/cooldowns."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot activate GA_Ritual_Start: ASC is null or GA_Ritual_Start_Class is not set."));
	}
}
