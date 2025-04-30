// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Interaction/IInteractableTarget.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class WITCHPT_API AItem : public AActor, public IInteractableTarget
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere)
	FInteractionOption Option;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& OptionBuilder) override;
};
