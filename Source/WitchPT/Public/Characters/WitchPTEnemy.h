// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WitchPTCharacterBase.h"
#include "WitchPTEnemy.generated.h"

UCLASS()
class WITCHPT_API AWitchPTEnemy : public AWitchPTCharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWitchPTEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
