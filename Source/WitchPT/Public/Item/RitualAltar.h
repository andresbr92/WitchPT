// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h" // Include for FGameplayTag
#include "RitualInterface.h"
#include "RitualAltar.generated.h"


class ARitualPosition;
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
	Inactive	UMETA(DisplayName = "Inavtive"),
	Preparing	UMETA(DisplayName = "Preparing"), // Players occupying positions
	Active		UMETA(DisplayName = "Active"), // Ritual sequence running
	Succeeded	UMETA(DisplayName = "Succeeded"),
	Failed		UMETA(DisplayName = "Failed"), // Generic fail state if needed
	FailedCatastrophically UMETA(DisplayName = "Failed Catastrophically")
};


UCLASS()
class WITCHPT_API ARitualAltar : public AActor, public IRitualInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARitualAltar();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Ritual")
	TArray<FGameplayTag> InputSequence;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Ritual")
	int32 CurrentSequenceIndex;

	UPROPERTY(Replicated, BlueprintReadWrite, VisibleAnywhere, Category = "Portal")
	TArray<TObjectPtr<ACharacter>> ParticipatingPlayers;
	
	UPROPERTY(Replicated, BlueprintReadWrite, VisibleAnywhere, Category = "Portal")
	float CurrentInputTimer;
	
	UPROPERTY(Replicated, BlueprintReadWrite, VisibleAnywhere, Category = "Portal")
	float CorruptionAmount;

	UPROPERTY(Replicated, BlueprintReadWrite, VisibleAnywhere, Category = "Portal")
	float CorruptionIncreasePerFail;
	
	UPROPERTY(Replicated, BlueprintReadWrite, VisibleAnywhere, Category = "Portal")
	float BaseInputTimeWindow;
	
	UPROPERTY(Replicated, BlueprintReadWrite, VisibleAnywhere, Category = "Portal")
	float DifficultyScalingMultiplier;
	
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_StartRitual(ACharacter* InitiatingPlayer);
	
	UFUNCTION(Server, Reliable)
	void Server_HandlePlayerInput(ACharacter* Character,const  FGameplayTag& InputTag);
	UFUNCTION(Server, Reliable)
	void Server_OccupyPosition(ACharacter* Player, ARitualPosition* Position);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnRitualStateChanged(ERitualState NewState);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnInputSuccess(ACharacter* Player);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnInputFailed(ACharacter* Player);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnRitualSucceeded();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnRitualCatastrophicFail();


	// Interface Implementation
	UFUNCTION(BlueprintCallable)
	virtual bool StartRitual(ACharacter* Character) override;
	

protected:
	

};
