// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Net/UnrealNetwork.h"
#include "WitchPTEquipmentInstance.generated.h"

struct FWitchPTEquipmentActorToSpawn;
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class WITCHPT_API UWitchPTEquipmentInstance : public UObject
{
	GENERATED_BODY()
public:
	UWitchPTEquipmentInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	

	// Equipment lifecycle
	// UObject Interface
	virtual bool IsSupportedForNetworking() const override { return true; }
	virtual UWorld* GetWorld() const override final;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// End of UObject Interface
	virtual void SpawnEquipmentActors(const TArray<FWitchPTEquipmentActorToSpawn>& ActorsToSpawn);
	virtual void DestroyEquipmentActors();
	virtual void OnEquipped();
	virtual void OnUnequipped();

	UFUNCTION(BlueprintPure, Category=Equipment)
	APawn* GetPawn() const;
	UFUNCTION(BlueprintPure, Category=Equipment)
	UObject* GetInstigator() const { return Instigator; }

	void SetInstigator(UObject* InInstigator) { Instigator = InInstigator; }
protected:
	UFUNCTION(BlueprintImplementableEvent, Category=Equipment)
	void K2_OnEquipped();
    
	UFUNCTION(BlueprintImplementableEvent, Category=Equipment)
	void K2_OnUnequipped();

private:
	UPROPERTY(ReplicatedUsing=OnRep_Instigator)
	TObjectPtr<UObject> Instigator;
	
	UPROPERTY(Replicated)
	TArray<TObjectPtr<AActor>> SpawnedActors;

	// Add rep notify:
	UFUNCTION()
	void OnRep_Instigator();
	
};
