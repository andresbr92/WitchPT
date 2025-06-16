// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Potion/PotionBase.h"


// Sets default values
APotionBase::APotionBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void APotionBase::GatherInteractionOptions(const FInteractionQuery& InteractQuery,
	FInteractionOptionBuilder& OptionBuilder)
{
	Super::GatherInteractionOptions(InteractQuery, OptionBuilder);
}

FItemManifest APotionBase::GetPickupInventory() const
{
	return StaticInventory;
}



