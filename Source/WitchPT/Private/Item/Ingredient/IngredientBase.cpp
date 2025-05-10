// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Ingredient/IngredientBase.h"


// Sets default values
AIngredientBase::AIngredientBase()
{
	
}

void AIngredientBase::GatherInteractionOptions(const FInteractionQuery& InteractQuery,
	FInteractionOptionBuilder& OptionBuilder)
{
	Super::GatherInteractionOptions(InteractQuery, OptionBuilder);
}

FInventoryPickup AIngredientBase::GetPickupInventory() const
{
	return StaticInventory;
}


