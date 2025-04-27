// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "WitchPTAttributeSet.generated.h"

/**
 * 
 */
// Macro for attribute accessors
// Functions: Getter, Setter, Initializer. The initializer set the base and current value!
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

// This syntax is a fucking nightmare, but...ok
UCLASS()
class WITCHPT_API UWitchPTAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	UWitchPTAttributeSet();
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;


	/**
	 * Vital Attributes
	 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Age, Category= "Vital Attributes")
	FGameplayAttributeData Age;
	ATTRIBUTE_ACCESSORS(UWitchPTAttributeSet, Age);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxAge, Category= "Vital Attributes")
	FGameplayAttributeData MaxAge;
	ATTRIBUTE_ACCESSORS(UWitchPTAttributeSet, MaxAge);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category= "Vital Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UWitchPTAttributeSet, Health);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category= "Secondary Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UWitchPTAttributeSet, MaxHealth);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category= "Vital Attributes")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UWitchPTAttributeSet, Strength);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxStrength, Category= "Vital Attributes")
	FGameplayAttributeData MaxStrength;
	ATTRIBUTE_ACCESSORS(UWitchPTAttributeSet, MaxStrength);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category= "Vital Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UWitchPTAttributeSet, Mana);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category= "Vital Attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UWitchPTAttributeSet, MaxMana);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Stamina, Category= "Vital Attributes")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UWitchPTAttributeSet, Stamina);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxStamina, Category= "Vital Attributes")
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UWitchPTAttributeSet, MaxStamina);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_SpeedMultiplier, Category= "Vital Attributes")
	FGameplayAttributeData SpeedMultiplier;
	ATTRIBUTE_ACCESSORS(UWitchPTAttributeSet, SpeedMultiplier);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxSpeedMultiplier, Category= "Vital Attributes")
	FGameplayAttributeData MaxSpeedMultiplier;
	ATTRIBUTE_ACCESSORS(UWitchPTAttributeSet, MaxSpeedMultiplier);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MoveSpeed, Category= "Vital Attributes")
	FGameplayAttributeData MoveSpeed;
	ATTRIBUTE_ACCESSORS(UWitchPTAttributeSet, MoveSpeed);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMoveSpeed, Category= "Vital Attributes")
	FGameplayAttributeData MaxMoveSpeed;
	ATTRIBUTE_ACCESSORS(UWitchPTAttributeSet, MaxMoveSpeed);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MinMoveSpeed, Category= "Vital Attributes")
	FGameplayAttributeData MinMoveSpeed;
	ATTRIBUTE_ACCESSORS(UWitchPTAttributeSet, MinMoveSpeed);

	


	// Replication functions
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;
	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;
	UFUNCTION()
	void OnRep_Stamina(const FGameplayAttributeData& OldStamina) const;
	UFUNCTION()
	void OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina) const;
	UFUNCTION()
	void OnRep_Age(const FGameplayAttributeData& OldAge) const;
	UFUNCTION()
	void OnRep_MaxAge(const FGameplayAttributeData& OldMaxAge) const;
	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;
	UFUNCTION()
	void OnRep_MaxStrength(const FGameplayAttributeData& OldMaxStrength) const;
	UFUNCTION()
	void OnRep_SpeedMultiplier(const FGameplayAttributeData& OldSpeedMultiplier) const;
	UFUNCTION()
	void OnRep_MaxSpeedMultiplier(const FGameplayAttributeData& OldMaxSpeedMultiplier) const;
	UFUNCTION()
	void OnRep_MoveSpeed(const FGameplayAttributeData& OldMoveSpeed) const;
	UFUNCTION()
	void OnRep_MaxMoveSpeed(const FGameplayAttributeData& OldMaxMoveSpeed) const;
	UFUNCTION()
	void OnRep_MinMoveSpeed(const FGameplayAttributeData& OldMinMoveSpeed) const;
};


