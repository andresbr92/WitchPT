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
	/**
	 * Constructor for the inventory manager component
	 * Sets up the component for replication and initializes the inventory list
	 */
	UWitchPTInventoryManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	 * Attempts to add an item to the inventory based on its definition
	 * If an item of the same type already exists, it will stack with it instead
	 * 
	 * @param ItemDef - The definition class of the item to add
	 * @return bool - True if a new item was added, false if it was stacked with an existing item
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Inventory")
	bool TryAddItemDefinition(TSubclassOf<UWitchPTInventoryItemDefinition> ItemDef);

	/**
	 * Server RPC to add a new item to the inventory
	 * Creates a new item instance and adds it to the inventory list
	 * 
	 * @param ItemDef - The definition class of the item to add
	 */
	UFUNCTION(Server, Reliable)
	void Server_AddItem(TSubclassOf<UWitchPTInventoryItemDefinition> ItemDef);

	/**
	 * Server RPC to add stacks to an existing item in the inventory
	 * Finds the first item matching the definition and increases its stack count
	 * 
	 * @param ItemDef - The definition class of the item to add stacks to
	 */
	UFUNCTION(Server, Reliable)
	void Server_AddStacksToItem(TSubclassOf<UWitchPTInventoryItemDefinition> ItemDef);

	/**
	 * Server RPC to update the stack count of a specific item instance
	 * Updates the count and marks the item for replication
	 * 
	 * @param ItemInstance - The specific item instance to update
	 * @param NewCount - The new stack count to set
	 */
	UFUNCTION(Server, Reliable)
	void Server_UpdateItemStackCount(UWitchPTInventoryItemInstance* ItemInstance, int32 NewCount);

	// UFUNCTION(Server, Reliable)
	// void RemoveItemInstance(UWitchPTInventoryItemInstance* ItemInstance);

	/**
	 * Get all items currently in the inventory
	 * 
	 * @return TArray<UWitchPTInventoryItemInstance*> - Array of all item instances in the inventory
	 */
	UFUNCTION(BlueprintCallable, Category=Inventory, BlueprintPure=false)
	TArray<UWitchPTInventoryItemInstance*> GetAllItems() const;

	/**
	 * Find the first item in the inventory that matches the given definition
	 * 
	 * @param ItemDef - The definition class to search for
	 * @return UWitchPTInventoryItemInstance* - The first matching item instance, or nullptr if none found
	 */
	UFUNCTION(BlueprintCallable, Category=Inventory, BlueprintPure)
	UWitchPTInventoryItemInstance* FindFirstItemStackByDefinition(TSubclassOf<UWitchPTInventoryItemDefinition> ItemDef) const;

	/**
	 * Get the total count of items matching a specific definition
	 * 
	 * @param ItemDef - The definition class to count
	 * @return int32 - The total count of matching items
	 */
	int32 GetTotalItemCountByDefinition(TSubclassOf<UWitchPTInventoryItemDefinition> ItemDef) const;
	
	/**
	 * Consume (remove) a specific number of items matching a definition
	 * 
	 * @param ItemDef - The definition class of items to consume
	 * @param NumToConsume - The number of items to consume
	 * @return bool - True if the requested number of items were successfully consumed
	 */
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
	//~End of UObject interface

private:
	/**
	 * The replicated list of inventory items
	 * Uses a fast array serializer for efficient replication
	 */
	UPROPERTY(Replicated)
	FWitchPTInventoryList InventoryList;
	UPROPERTY()
	TObjectPtr<UInventoryUserWidget> InventoryMenu;


	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UInventoryUserWidget> InventoryMenuClass;

	TWeakObjectPtr<APlayerController> OwningController;
	UFUNCTION(BlueprintCallable, Category= "Inventory")
	void ToggleInventoryMenu();
	void ConstructInventory();
	bool bInventoryMenuOpen;
	void OpenInventoryMenu();
	void CloseInventoryMenu();
};
