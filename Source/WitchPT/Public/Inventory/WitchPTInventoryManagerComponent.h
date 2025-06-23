// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FastArray/FastArrayList.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "WitchPTInventoryManagerComponent.generated.h"

class UInventoryUserWidget;
class UWitchPTInventoryItemDefinition;
class UWitchPTInventoryItemInstance;
class UWitchPTInventoryManagerComponent;
class UObject;
struct FFrame;
struct FNetDeltaSerializeInfo;
struct FWitchPTInventoryList;
struct FReplicationFlags;

/**
 * Delegate for inventory item events.
 * Used to broadcast when items are added, removed, or their stack counts change.
 * @param UWitchPTInventoryItemInstance* - The item instance that was affected
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemChanged, UWitchPTInventoryItemInstance*);

/**
 * WitchPTInventoryManagerComponent
 * 
 * A component that manages a player's inventory of items.
 * Handles adding, removing, and stacking items, as well as replicating inventory state across the network.
 * Uses a fast array serializer for efficient replication of inventory items.
 */
UCLASS(BlueprintType)
class WITCHPT_API UWitchPTInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

public:
	
	UWitchPTInventoryManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Inventory")
	bool TryAddItemDefinition(TSubclassOf<UWitchPTInventoryItemDefinition> ItemDef);

	
	UFUNCTION(Server, Reliable)
	void Server_AddItem(TSubclassOf<UWitchPTInventoryItemDefinition> ItemDef);

	
	UFUNCTION(Server, Reliable)
	void Server_AddStacksToItem(TSubclassOf<UWitchPTInventoryItemDefinition> ItemDef);

	
	UFUNCTION(Server, Reliable)
	void Server_UpdateItemStackCount(UWitchPTInventoryItemInstance* ItemInstance, int32 NewCount);

	
	UFUNCTION(Server, Reliable)
	void Server_RemoveItemInstance(UWitchPTInventoryItemInstance* ItemInstance);


	UFUNCTION(Server, Reliable)
	void Server_RemoveItemStacks(UWitchPTInventoryItemInstance* ItemInstance, int32 AmountToRemove);

	
	UFUNCTION(BlueprintCallable, Category=Inventory, BlueprintPure=false)
	TArray<UWitchPTInventoryItemInstance*> GetAllItems() const;

	
	UFUNCTION(BlueprintCallable, Category=Inventory, BlueprintPure)
	UWitchPTInventoryItemInstance* FindFirstItemStackByDefinition(TSubclassOf<UWitchPTInventoryItemDefinition> ItemDef) const;

	
	int32 GetTotalItemCountByDefinition(TSubclassOf<UWitchPTInventoryItemDefinition> ItemDef) const;
	
	
	bool ConsumeItemsByDefinition(TSubclassOf<UWitchPTInventoryItemDefinition> ItemDef, int32 NumToConsume);

	// Delegates for inventory events
	
	/** Fired when a new item is added to the inventory */
	FOnItemChanged OnItemAdded;
	
	/** Fired when an item's stack count changes */
	FOnItemChanged OnItemStackChanged;
	
	/** Fired when an item is removed from the inventory */
	FOnItemChanged OnItemRemoved;

	//~UObject interface
	/**
	 * Replicates all inventory item instances as subobjects
	 * Required for proper replication of the inventory system
	 */
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	
	/**
	 * Called when the component is ready for replication
	 * Registers all existing inventory items for replication
	 */
	virtual void ReadyForReplication() override;




private:
	/**
	 * The replicated list of inventory items
	 * Uses a fast array serializer for efficient replication
	 */
	UPROPERTY(Replicated)
	FWitchPTInventoryList InventoryList;
	

	TWeakObjectPtr<APlayerController> OwningController;
	

};