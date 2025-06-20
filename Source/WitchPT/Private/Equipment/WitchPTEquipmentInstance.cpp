// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/WitchPTEquipmentInstance.h"

UWitchPTEquipmentInstance::UWitchPTEquipmentInstance(const FObjectInitializer& ObjectInitializer)
{
}

void UWitchPTEquipmentInstance::OnEquipped()
{
	UE_LOG(LogTemp, Warning, TEXT("✅ EQUIPPED: %s"), *GetClass()->GetName());
	K2_OnEquipped();
}

void UWitchPTEquipmentInstance::OnUnequipped()
{
	UE_LOG(LogTemp, Warning, TEXT("❌ UNEQUIPPED: %s"), *GetClass()->GetName());
	K2_OnUnequipped();
}

APawn* UWitchPTEquipmentInstance::GetPawn() const
{
	return Cast<APawn>(GetOuter());
}
