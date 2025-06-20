// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/WitchPTEquipmentManagerComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Equipment/WitchPTEquipmentDefinition.h"
#include "Equipment/WitchPTEquipmentInstance.h"


// Sets default values for this component's properties
UWitchPTEquipmentManagerComponent::UWitchPTEquipmentManagerComponent()
{
	
}

UWitchPTEquipmentInstance* UWitchPTEquipmentManagerComponent::EquipItem(
	TSubclassOf<UWitchPTEquipmentDefinition> EquipmentDefinition)
{
	if (!EquipmentDefinition) return nullptr;
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn) return nullptr;
	const UWitchPTEquipmentDefinition* EquipmentCDO = GetDefault<UWitchPTEquipmentDefinition>(EquipmentDefinition);
	TSubclassOf<UWitchPTEquipmentInstance> InstanceType = EquipmentCDO->InstanceType;
	if (!InstanceType) InstanceType = UWitchPTEquipmentInstance::StaticClass();
	UWitchPTEquipmentInstance* NewInstance = NewObject<UWitchPTEquipmentInstance>(OwnerPawn, InstanceType);
	EquippedItems.Add(NewInstance);
	NewInstance->OnEquipped();

	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		FEquipmentAbilityHandles& AbilityHandles = EquipmentAbilities.Add(NewInstance);
		for (TSubclassOf<UGameplayAbility> AbilityClass : EquipmentCDO->AbilitiesToGrant)
		{
			if (AbilityClass)
			{
				FGameplayAbilitySpec AbilitySpec(AbilityClass, 1, INDEX_NONE, NewInstance);
				FGameplayAbilitySpecHandle SpecHandle = ASC->GiveAbility(AbilitySpec);
				AbilityHandles.GrantedHandles.Add(SpecHandle);
				UE_LOG(LogTemp, Warning, TEXT("🎯 Granted ability: %s"), *AbilityClass->GetName());
			}
		}
		
	}
    
	return NewInstance;
}

void UWitchPTEquipmentManagerComponent::UnequipItem(UWitchPTEquipmentInstance* ItemInstance)
{
	if (!ItemInstance) return;
    
	ItemInstance->OnUnequipped();
	EquippedItems.Remove(ItemInstance);
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		if (FEquipmentAbilityHandles* AbilityHandles = EquipmentAbilities.Find(ItemInstance))
		{
			for (const FGameplayAbilitySpecHandle& Handle : AbilityHandles->GrantedHandles)
			{
				if (Handle.IsValid())
				{
					ASC->ClearAbility(Handle);
					UE_LOG(LogTemp, Warning, TEXT("🚫 Removed ability"));
				}
			}
			EquipmentAbilities.Remove(ItemInstance);
		}
	}
}

void UWitchPTEquipmentManagerComponent::PrintEquippedItems()
{
	UE_LOG(LogTemp, Warning, TEXT("📦 EQUIPPED ITEMS: %d total"), EquippedItems.Num());
	for (int32 i = 0; i < EquippedItems.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("  [%d]: %s"), i, EquippedItems[i] ? *EquippedItems[i]->GetClass()->GetName() : TEXT("NULL"));
	}
}

UAbilitySystemComponent* UWitchPTEquipmentManagerComponent::GetAbilitySystemComponent() const
{
	if (APawn* OwnerPawn = Cast<APawn>(GetOwner()))
	{
		return UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwnerPawn);
	}
	return nullptr;
}



