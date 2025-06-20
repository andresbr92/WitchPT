// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/WitchPTEquipmentDefinition.h"

#include "Equipment/WitchPTEquipmentInstance.h"

UWitchPTEquipmentDefinition::UWitchPTEquipmentDefinition(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstanceType = UWitchPTEquipmentInstance::StaticClass();
}
