// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "WitchPTCharacterBase.generated.h"

class UWitchPTAbilitySystemComponent;
class UWitchPTAttributeSet;
class UGameplayAbility;
class UGameplayEffect;
class UAttributeSet;

UCLASS(Abstract)
class WITCHPT_API AWitchPTCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWitchPTCharacterBase(const class FObjectInitializer& ObjectInitializer);

	// Getter for ASC Main Components
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UWitchPTAttributeSet* GetAttributeSet() { return AttributeSet; }

protected:
	UPROPERTY()
	TObjectPtr<UWitchPTAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UWitchPTAttributeSet> AttributeSet;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultAttributes;
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	// These effects are only applied one time on startup
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GASDocumentation|Abilities")
	TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;

	virtual void InitializeDefaultAttributes();
	
	virtual void GrantStartupAbilities();

	virtual void AddStartupEffects();

	virtual void InitAbilityActorInfo();
private:
	virtual void ApplyGameplayEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass);



public:
	/**
	* Getters for attributes from GDAttributeSetBase
	**/
	UFUNCTION(BlueprintCallable, Category = "WitchPT|WitchPTCharacter|Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "WitchPT|WitchPTCharacter|Attributes")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "WitchPT|WitchPTCharacter|Attributes")
	float GetStamina() const;

	UFUNCTION(BlueprintCallable, Category = "WitchPT|WitchPTCharacter|Attributes")
	float GetMaxStamina() const;

	UFUNCTION(BlueprintCallable, Category = "WitchPT|WitchPTCharacter|Attributes")
	float GetAge() const;

	UFUNCTION(BlueprintCallable, Category = "WitchPT|WitchPTCharacter|Attributes")
	float GetMaxAge() const;

	UFUNCTION(BlueprintCallable, Category = "WitchPT|WitchPTCharacter|Attributes")
	float GetMana() const;

	UFUNCTION(BlueprintCallable, Category = "WitchPT|WitchPTCharacter|Attributes")
	float GetMaxMana() const;

	UFUNCTION(BlueprintCallable, Category = "WitchPT|WitchPTCharacter|Attributes")
	float GetSpeedMultiplier() const;

	UFUNCTION(BlueprintCallable, Category = "WitchPT|WitchPTCharacter|Attributes")
	float GetMoveSpeed() const;

	UFUNCTION(BlueprintCallable, Category = "WitchPT|WitchPTCharacter|Attributes")
	float GetMaxMoveSpeed() const;

	

	

	

};
