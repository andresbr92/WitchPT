// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h" // Include for IAbilitySystemInterface
#include "GameplayTagContainer.h" // Include for FGameplayTag
#include "RitualAltar.generated.h"

// Forward Declarations
class UAbilitySystemComponent;
class URitualAttributeSet;
class ARitualPosition;
class ACharacter;
class UGameplayAbility;
class UGameplayEffect;
// enum class ERitualInput : uint8; // Assuming ERitualInput is defined elsewhere - DEFINING IT HERE NOW
class UBoxComponent; // Added Forward Declaration

// Enum defining the possible inputs for the ritual sequence
UENUM(BlueprintType)
enum class ERitualInput : uint8
{
	Up		UMETA(DisplayName = "Up"),
	Down	UMETA(DisplayName = "Down"),
	Left	UMETA(DisplayName = "Left"),
	Right	UMETA(DisplayName = "Right"),
	None	UMETA(DisplayName = "None") // Optional: For default/invalid state
};

UENUM(BlueprintType)
enum class ERitualState : uint8
{
	Idle		UMETA(DisplayName = "Idle"),
	Preparing	UMETA(DisplayName = "Preparing"), // Players occupying positions
	Active		UMETA(DisplayName = "Active"), // Ritual sequence running
	Succeeded	UMETA(DisplayName = "Succeeded"),
	Failed		UMETA(DisplayName = "Failed"), // Generic fail state if needed
	FailedCatastrophically UMETA(DisplayName = "Failed Catastrophically")
};


UCLASS()
class WITCHPT_API ARitualAltar : public AActor, public IAbilitySystemInterface // Implement the interface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARitualAltar();

	//~ Begin AActor Interface
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Tick(float DeltaTime) override;
	//~ End AActor Interface

	//~ Begin IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override; // Implement the interface function
	//~ End IAbilitySystemInterface

	//~ Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<URitualAttributeSet> AttributeSet;

	// Volume to detect deposited NPC
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> DepositVolume;
	//~ End Components

	//~ Ritual State & Data (Replicated)
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Ritual|State")
	TArray<ERitualInput> CurrentSequence;

	UPROPERTY(ReplicatedUsing = OnRep_ParticipatingPlayers, BlueprintReadOnly, Category = "Ritual|State", VisibleAnywhere)
	TArray<TObjectPtr<ACharacter>> ParticipatingPlayers;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Ritual|State")
	TObjectPtr<ACharacter> CurrentActivePlayer;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Ritual|Positions")
	TArray<TObjectPtr<ARitualPosition>> RitualPositions; // Assuming positions are known/assigned
	//~ End Ritual State & Data

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ritual|Positions")
	TMap<TObjectPtr<ARitualPosition>, TObjectPtr<AActor>> RitualPositionsMap;

	//~ Gameplay Abilities (Assigned in Blueprint)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ritual|Abilities")
	TSubclassOf<UGameplayAbility> GA_Ritual_Start_Class;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ritual|Abilities")
	TSubclassOf<UGameplayAbility> GA_Ritual_AdvanceTurn_Class;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ritual|Abilities")
	TSubclassOf<UGameplayAbility> GA_Ritual_FailInput_Class;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ritual|Abilities")
	TSubclassOf<UGameplayAbility> GA_Ritual_Succeed_Class;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ritual|Abilities")
	TSubclassOf<UGameplayAbility> GA_Ritual_CatastrophicFail_Class;
	//~ End Gameplay Abilities

	//~ Gameplay Effects (Assigned in Blueprint)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ritual|Effects")
	TSubclassOf<UGameplayEffect> GE_Ritual_IncreaseCorruption_Class;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ritual|Effects")
	TSubclassOf<UGameplayEffect> GE_Ritual_SetState_Active_Class;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ritual|Effects")
	TSubclassOf<UGameplayEffect> GE_Ritual_SetState_Succeeded_Class;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ritual|Effects")
	TSubclassOf<UGameplayEffect> GE_Ritual_SetState_FailedCatastrophically_Class;
	//~ End Gameplay Effects

	//~ RPCs
	UFUNCTION(Server, Reliable)
	void Server_OccupyPosition(ACharacter* Player, ARitualPosition* Position);
	//~ End RPCs

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnRep_ParticipatingPlayers(); // Optional: For specific logic when the array replicates

	// Called when something overlaps the DepositVolume
	UFUNCTION()
	void OnDepositVolumeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
