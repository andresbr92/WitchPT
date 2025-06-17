// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Potion/PotionBase.h"


// Sets default values
APotionBase::APotionBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Set default ability path
	InteractionAbilityClass = TSoftClassPtr<UGameplayAbility>(FSoftObjectPath(TEXT("/Game/Blueprints/AbilitySystem/GameplayAbilities/Interact/GA_Interact_Collect.GA_Interact_Collect_C")));
}

void APotionBase::GatherInteractionOptions(const FInteractionQuery& InteractQuery,
	FInteractionOptionBuilder& OptionBuilder)
{
	// Load the ability class if not already loaded
	if (!LoadedInteractionAbility && !InteractionAbilityClass.IsNull())
	{
		LoadedInteractionAbility = InteractionAbilityClass.LoadSynchronous();
	}
	
	// Set up the interaction option
	if (LoadedInteractionAbility)
	{
		Option.InteractionAbilityToGrant = LoadedInteractionAbility;
		Option.Text = FText::FromString(TEXT("Pick up Potion"));
		// Option.SubText = FText::FromString(TEXT("Add to inventory"));
	}
	Super::GatherInteractionOptions(InteractQuery, OptionBuilder);
}

FItemManifest APotionBase::GetPickupInventory() const
{
	return StaticInventory;
}



