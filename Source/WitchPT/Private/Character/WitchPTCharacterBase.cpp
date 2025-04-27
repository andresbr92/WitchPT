// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/WitchPTCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/WitchPTAbilitySystemComponent.h"
#include "AbilitySystem/WitchPTAttributeSet.h"
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

void AWitchPTCharacterBase::AddStartupEffects()
{
	if (GetLocalRole() != ROLE_Authority || !IsValid(AbilitySystemComponent) || AbilitySystemComponent->bStartupEffectsApplied)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (TSubclassOf<UGameplayEffect> GameplayEffect : StartupEffects)
	{
		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 1.f, EffectContext);
		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
		}
	}

	AbilitySystemComponent->bStartupEffectsApplied = true;
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

float AWitchPTCharacterBase::GetHealth() const
{
	if (IsValid(AttributeSet))
	{
		return AttributeSet->GetHealth();
	}
	return 0.f;
}

float AWitchPTCharacterBase::GetMaxHealth() const
{
	if (IsValid(AttributeSet))
	{
		return AttributeSet->GetMaxHealth();
	}
	return 0.f;
}

float AWitchPTCharacterBase::GetStamina() const
{
	if (IsValid(AttributeSet))	
	{
		return AttributeSet->GetStamina();
	}
	return 0.f;
}

float AWitchPTCharacterBase::GetMaxStamina() const
{
	if (IsValid(AttributeSet))
	{
		return AttributeSet->GetMaxStamina();
	}
	return 0.f;
}

float AWitchPTCharacterBase::GetAge() const
{
	if (IsValid(AttributeSet))
	{
		return AttributeSet->GetAge();
	}
	return 0.f;
}

float AWitchPTCharacterBase::GetMaxAge() const
{
	if (IsValid(AttributeSet))
	{
		return AttributeSet->GetMaxAge();
	}
	return 0.f;
}

float AWitchPTCharacterBase::GetMana() const
{
	if (IsValid(AttributeSet))
	{
		return AttributeSet->GetMana();
	}
	return 0.f;
}

float AWitchPTCharacterBase::GetMaxMana() const
{
	if (IsValid(AttributeSet))
	{
		return AttributeSet->GetMaxMana();
	}
	return 0.f;
}

float AWitchPTCharacterBase::GetSpeedMultiplier() const
{
	if (IsValid(AttributeSet))
	{
		return AttributeSet->GetSpeedMultiplier();
	}
	return 0.f;
}

float AWitchPTCharacterBase::GetMoveSpeed() const
{
	if (IsValid(AttributeSet))
	{
		return AttributeSet->GetMoveSpeed();
	}
	return 0.f;
}

float AWitchPTCharacterBase::GetMaxMoveSpeed() const
{
	if (IsValid(AttributeSet))
	{
		return AttributeSet->GetMaxMoveSpeed();
	}
	return 0.f;
}

