// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Collectable.h"

#include "Inventory/WitchPTInventoryManagerComponent.h"

UCollectableStatics::UCollectableStatics()
: Super(FObjectInitializer::Get())
{
}

TScriptInterface<ICollectable> UCollectableStatics::GetFirstPickupableFromActor(AActor* Actor)
{
	TScriptInterface<ICollectable> PickupableActor(Actor);
	if (PickupableActor)
	{
		return PickupableActor;
	}
	
	TArray<UActorComponent*> PickupableComponents = Actor ? Actor->GetComponentsByInterface(UCollectable::StaticClass()) : TArray<UActorComponent*>();
	if (PickupableComponents.Num() > 0)
	{
		return TScriptInterface<ICollectable>(PickupableComponents[0]);
	}

	return TScriptInterface<ICollectable>();
}


void UCollectableStatics::AddPickupToInventory(UWitchPTInventoryManagerComponent* InventoryComponent,
	TScriptInterface<ICollectable> Collectable)
{
	if (InventoryComponent && Collectable)
	{
		const FItemManifest& ItemManifest = Collectable->GetPickupInventory();
		InventoryComponent->TryAddItemDefinition(ItemManifest.ItemDef);
	}
}
