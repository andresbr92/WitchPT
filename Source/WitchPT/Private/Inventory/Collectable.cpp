// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Collectable.h"

#include "Inventory/WitchPTInventoryManagerComponent.h"

UCollectableStatics::UCollectableStatics()
: Super(FObjectInitializer::Get())
{
}

TScriptInterface<ICollectable> UCollectableStatics::GetFirstPickupableFromActor(AActor* Actor)
{
	// If the actor is directly pickupable, return that.
	TScriptInterface<ICollectable> PickupableActor(Actor);
	if (PickupableActor)
	{
		return PickupableActor;
	}

	// If the actor isn't pickupable, it might have a component that has a pickupable interface.
	TArray<UActorComponent*> PickupableComponents = Actor ? Actor->GetComponentsByInterface(UCollectable::StaticClass()) : TArray<UActorComponent*>();
	if (PickupableComponents.Num() > 0)
	{
		// Get first pickupable, if the user needs more sophisticated pickup distinction, will need to be solved elsewhere.
		return TScriptInterface<ICollectable>(PickupableComponents[0]);
	}

	return TScriptInterface<ICollectable>();
}

// Add default functionality here for any ICollectable functions that are not pure virtual.
void UCollectableStatics::AddPickupToInventory(UWitchPTInventoryManagerComponent* InventoryComponent,
	TScriptInterface<ICollectable> Collectable)
{
	if (InventoryComponent && Collectable)
	{
		const FInventoryPickup& PickupInventory = Collectable->GetPickupInventory();

		for (const FPickupTemplate& Template : PickupInventory.Templates)
		{
			InventoryComponent->AddItemDefinition(Template.ItemDef, Template.StackCount);
		}

		for (const FPickupInstance& Instance : PickupInventory.Instances)
		{
			InventoryComponent->AddItemInstance(Instance.Item);
		}
	}
}
