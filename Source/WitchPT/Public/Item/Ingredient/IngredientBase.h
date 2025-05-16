// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/Collectable.h"
#include "Item/Item.h"
#include "IngredientBase.generated.h"

UCLASS()
class WITCHPT_API AIngredientBase : public AItem, public ICollectable
{
	GENERATED_BODY()

public:

	AIngredientBase();
	virtual void GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& OptionBuilder) override;
	virtual FItemManifest GetPickupInventory() const override;
	
protected:

	UPROPERTY(EditAnywhere)
	FItemManifest StaticInventory;

};
