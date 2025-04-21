// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/WitchPTCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/WitchPTAbilitySystemComponent.h"
#include "Character/Components/WitchPTCharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AWitchPTCharacterBase::AWitchPTCharacterBase(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UWitchPTCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

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

void AWitchPTCharacterBase::GrantStartupAbilities()
{
	if (!HasAuthority()) return;
	
	UWitchPTAbilitySystemComponent* WitchPtAbilitySystemComponent = Cast<UWitchPTAbilitySystemComponent>(AbilitySystemComponent);
	
	WitchPtAbilitySystemComponent->GrantStartupAbilities(StartupAbilities);
}

void AWitchPTCharacterBase::InitAbilityActorInfo()
{
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

