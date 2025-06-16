// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/Fragments/WitchPTInventoryItemFragment.h"
#include "WitchPTInventoryItemFragment_WorldDetails.generated.h"

/**
 * Fragment that defines how items appear in the world
 * This stores mesh and visual information for world representation
 */
UCLASS()
class WITCHPT_API UWitchPTInventoryItemFragment_WorldDetails : public UWitchPTInventoryItemFragment
{
	GENERATED_BODY()

public:
	// Static mesh used when item is spawned in the world
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "World Representation")
	UStaticMesh* WorldStaticMesh;

	// Optional material override for the world mesh
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "World Representation")
	UMaterialInterface* WorldMaterial;

	// Scale applied to the mesh when spawned in world
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "World Representation")
	FVector WorldScale = FVector(1.0f, 1.0f, 1.0f);

	// Collision profile name for the world object
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "World Representation")
	FName CollisionProfileName = "Item";

	// Helper functions
	UFUNCTION(BlueprintPure, Category = "World Details")
	bool HasValidWorldMesh() const;

	virtual void OnInstanceCreated(UWitchPTInventoryItemInstance* Instance) const override;
}; 