// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/WitchPTCharacterBase.h"

// Sets default values
AWitchPTCharacterBase::AWitchPTCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWitchPTCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWitchPTCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AWitchPTCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

