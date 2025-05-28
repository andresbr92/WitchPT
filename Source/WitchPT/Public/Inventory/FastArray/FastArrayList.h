#pragma once

#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"

#include "FastArrayList.generated.h"

class UWitchPTInventoryItemDefinition;
class UInventoryItemDefinition;
class UWitchPTInventoryManagerComponent;
struct FWitchPTInventoryList;

/**
 * FWitchPTInventoryEntry
 * 
 * Represents a single item entry in the inventory.
 * Uses FastArraySerializer for efficient replication.
 */
USTRUCT(BlueprintType)
struct FWitchPTInventoryEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FWitchPTInventoryEntry()
	{}

	/**
	 * Get a debug string representation of this inventory entry
	 * @return FString - A string describing the item instance and its definition
	 */
	FString GetDebugString() const;

private:
	friend FWitchPTInventoryList;
	friend UWitchPTInventoryManagerComponent;

	/** The actual item instance this entry represents */
	UPROPERTY()
	TObjectPtr<UInventoryItemDefinition> Instance = nullptr;
	
};

/**
 * FWitchPTInventoryList
 * 
 * A collection of inventory entries that supports efficient replication.
 * Uses FastArraySerializer to minimize network traffic when replicating inventory changes.
 * Handles adding, removing, and updating inventory items.
 */
USTRUCT(BlueprintType)
struct FWitchPTInventoryList : public FFastArraySerializer
{
	GENERATED_BODY()

	/** Default constructor */
	FWitchPTInventoryList()
		: OwnerComponent(nullptr)
	{
	}

	/**
	 * Constructor with owner component
	 * @param InOwnerComponent - The component that owns this inventory list
	 */
	FWitchPTInventoryList(UActorComponent* InOwnerComponent)
		: OwnerComponent(InOwnerComponent)
	{
	}

	/**
	 * Get all item instances in the inventory
	 * @return TArray<UWitchPTInventoryItemInstance*> - Array of all item instances
	 */
	TArray<UInventoryItemDefinition*> GetAllItems() const;

public:
	//~FFastArraySerializer contract
	/**
	 * Called before items are removed during replication
	 * Broadcasts the OnItemRemoved event for each removed item
	 */
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	
	/**
	 * Called after items are added during replication
	 * Broadcasts the OnItemAdded event for each added item
	 */
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	
	/**
	 * Called after items are changed during replication
	 * Broadcasts the OnItemStackChanged event for each changed item
	 */
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer contract

	/**
	 * Serializes the delta between two states of the array
	 * Required for FastArraySerializer functionality
	 */
	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FWitchPTInventoryEntry, FWitchPTInventoryList>(Entries, DeltaParms, *this);
	}

	/**
	 * Create a new inventory entry with the specified item definition
	 * Initializes the item instance and sets up its stack count based on the definition
	 *
	 * @param ItemClass The class of the inventory item to add
	 * @return A pointer to the newly created inventory item instance
	 */
	UInventoryItemDefinition* AddEntry(TSubclassOf<UWitchPTInventoryItemDefinition> ItemClass);
	
	/**
	 * Adds an existing item instance to the inventory list
	 *
	 * @param Instance The inventory item instance to add
	 */
	void AddEntry(UInventoryItemDefinition* Instance);

	/**
	 * Removes an item instance from the inventory list
	 * 
	 * @param Instance The inventory item instance to remove
	 */
	void RemoveEntry(UInventoryItemDefinition* Instance);

private:
	friend UWitchPTInventoryManagerComponent;

private:
	/** Replicated list of inventory entries */
	UPROPERTY()
	TArray<FWitchPTInventoryEntry> Entries;

	/** Non-replicated reference to the owner component */
	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;
};

/**
 * Trait specification for FWitchPTInventoryList
 * Enables network delta serialization for efficient replication
 */
template<>
struct TStructOpsTypeTraits<FWitchPTInventoryList> : public TStructOpsTypeTraitsBase2<FWitchPTInventoryList>
{
	enum { WithNetDeltaSerializer = true };
};