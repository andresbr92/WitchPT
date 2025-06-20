// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WitchPTEquipmentManagerComponent.generated.h"


class UAbilitySystemComponent;
struct FGameplayAbilitySpecHandle;
class UWitchPTEquipmentDefinition;
class UWitchPTEquipmentInstance;

USTRUCT()
struct FEquipmentAbilityHandles
{
	GENERATED_BODY()
	TArray<FGameplayAbilitySpecHandle> GrantedHandles;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class WITCHPT_API UWitchPTEquipmentManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UWitchPTEquipmentManagerComponent();

	UFUNCTION(BlueprintCallable, Category="Equipment")
	UWitchPTEquipmentInstance* EquipItem(TSubclassOf<UWitchPTEquipmentDefinition> EquipmentDefinition);

	UFUNCTION(BlueprintCallable, Category="Equipment")
	void UnequipItem(UWitchPTEquipmentInstance* ItemInstance);

	UFUNCTION(BlueprintCallable, Category="Equipment")
	void PrintEquippedItems();

protected:
	UPROPERTY(VisibleAnywhere, Category="Equipment")
	TArray<TObjectPtr<UWitchPTEquipmentInstance>> EquippedItems;

	// Add after EquippedItems:
	UPROPERTY()
	TMap<TObjectPtr<UWitchPTEquipmentInstance>, FEquipmentAbilityHandles> EquipmentAbilities;

private:
	UAbilitySystemComponent* GetAbilitySystemComponent() const;


};
