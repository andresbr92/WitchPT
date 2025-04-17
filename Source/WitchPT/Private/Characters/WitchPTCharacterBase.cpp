// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/WitchPTCharacterBase.h"

#include "AbilitySystemComponent.h"

// Sets default values
AWitchPTCharacterBase::AWitchPTCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

UAbilitySystemComponent* AWitchPTCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}



// Called to bind functionality to input
void AWitchPTCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AWitchPTCharacterBase::InitializeDefaultAttributes()
{
	ApplyGameplayEffectToSelf(DefaultAttributes);
	
	
}

void AWitchPTCharacterBase::ApplyGameplayEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle EffectSpec = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffectClass, 1.f, EffectContextHandle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data.Get());
	
}

