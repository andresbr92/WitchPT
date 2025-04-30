// Fill out your copyright notice in the Description page of Project Settings.



#include "Item/RitualPosition.h"


// Sets default values
ARitualPosition::ARitualPosition()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

void ARitualPosition::GatherInteractionOptions(const FInteractionQuery& InteractQuery,
	FInteractionOptionBuilder& OptionBuilder)
{
	Super::GatherInteractionOptions(InteractQuery, OptionBuilder);
}



