// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/IPickupable.h"

#include "Inventory/WitchPTInventoryManagerComponent.h"

UCollectableStatics::UCollectableStatics()
: Super(FObjectInitializer::Get())
{
}

TScriptInterface<IIPickupable> UCollectableStatics::GetFirstPickupableFromActor(AActor* Actor)
{
	TScriptInterface<IIPickupable> PickupableActor(Actor);
	if (PickupableActor)
	{
		return PickupableActor;
	}
	
	TArray<UActorComponent*> PickupableComponents = Actor ? Actor->GetComponentsByInterface(UIPickupable::StaticClass()) : TArray<UActorComponent*>();
	if (PickupableComponents.Num() > 0)
	{
		return TScriptInterface<IIPickupable>(PickupableComponents[0]);
	}

	return TScriptInterface<IIPickupable>();
}


void UCollectableStatics::AddPickupToInventory(UWitchPTInventoryManagerComponent* InventoryComponent,
	TScriptInterface<IIPickupable> Collectable)
{
	if (InventoryComponent && Collectable)
	{
		const FInventoryPickup& PickupInventory = Collectable->GetPickupInventory();

		for (const FPickupTemplate& Template : PickupInventory.Templates)
		{
			InventoryComponent->TryAddItemDefinition(Template.ItemDef);
		}

		// for (const FPickupInstance& Instance : PickupInventory.Instances)
		// {
		// 	InventoryComponent->AddItemInstance(Instance.Item);
		// }
	}
}
