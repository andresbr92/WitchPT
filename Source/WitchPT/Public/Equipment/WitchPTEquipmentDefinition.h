// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "WitchPTEquipmentDefinition.generated.h"

class UWitchPTEquipmentInstance;
class UGameplayAbility;

USTRUCT()
struct FWitchPTEquipmentActorToSpawn
{
	GENERATED_BODY()
	FWitchPTEquipmentActorToSpawn() {};

	UPROPERTY(EditAnywhere, Category=Equipment)
	TSubclassOf<AActor> ActorToSpawn;

	UPROPERTY(EditAnywhere, Category=Equipment)
	FName AttachSocket;

	UPROPERTY(EditAnywhere, Category=Equipment)
	FTransform AttachTransform;
};


/**
 * UWitchPTEquipmentDefinition
 * Definition of a piece of equipment that can be applied to a pawn
 */
UCLASS(Blueprintable, BlueprintType)
class WITCHPT_API UWitchPTEquipmentDefinition : public UObject
{
	GENERATED_BODY()
public:
	UWitchPTEquipmentDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Class to spawn for equipment instance
	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TSubclassOf<UWitchPTEquipmentInstance> InstanceType;

	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TArray<TSubclassOf<UGameplayAbility>> AbilitiesToGrant;

	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TArray<FWitchPTEquipmentActorToSpawn> ActorsToSpawn;
};
