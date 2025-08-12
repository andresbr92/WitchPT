// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/WitchPTAbilitySystemLibrary.h"

#include "Item/MechanicsInterface.h"

TScriptInterface<IMechanicsInterface> UWitchPTAbilitySystemLibrary::GetMechanicsInterface(const AActor* Actor)
{
	if (!Actor)
	{
		return TScriptInterface<IMechanicsInterface>();
		
	}
	TArray<UActorComponent*> ComponentsImplementingInterface = Actor->GetComponentsByInterface(UMechanicsInterface::StaticClass());
	if (ComponentsImplementingInterface.Num() > 0)
	{
		return TScriptInterface<IMechanicsInterface>(ComponentsImplementingInterface[0]);
	}
	return TScriptInterface<IMechanicsInterface>();
}
