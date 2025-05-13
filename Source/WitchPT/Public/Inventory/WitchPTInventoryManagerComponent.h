// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "WitchPTInventoryManagerComponent.generated.h"

class UWitchPTInventoryItemDefinition;
class UWitchPTInventoryItemInstance;
class UWitchPTInventoryManagerComponent;
class UObject;
struct FFrame;
struct FNetDeltaSerializeInfo;
struct FWitchPTInventoryList;
struct FReplicationFlags;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemChanged, UWitchPTInventoryItemInstance*);
/** A message when an item is added to the inventory */
USTRUCT(BlueprintType)
struct FWitchPTInventoryChangeMessage
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly, Category=Inventory)
	TObjectPtr<UActorComponent> InventoryOwner = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	TObjectPtr<UWitchPTInventoryItemInstance> Instance = nullptr;

	UPROPERTY(BlueprintReadOnly, Category=Inventory)
	int32 NewCount = 0;

	UPROPERTY(BlueprintReadOnly, Category=Inventory)
	int32 Delta = 0;
};
USTRUCT(BlueprintType)
struct FWitchPTInventoryEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FWitchPTInventoryEntry()
	{}

	FString GetDebugString() const;

private:
	friend FWitchPTInventoryList;
	friend UWitchPTInventoryManagerComponent;

	UPROPERTY()
	TObjectPtr<UWitchPTInventoryItemInstance> Instance = nullptr;

	UPROPERTY()
	int32 StackCount = 0;

	UPROPERTY(NotReplicated)
	int32 LastObservedCount = INDEX_NONE;
};

/** List of inventory items */
USTRUCT(BlueprintType)
struct FWitchPTInventoryList : public FFastArraySerializer
{
	GENERATED_BODY()

	FWitchPTInventoryList()
		: OwnerComponent(nullptr)
	{
	}

	FWitchPTInventoryList(UActorComponent* InOwnerComponent)
		: OwnerComponent(InOwnerComponent)
	{
	}

	TArray<UWitchPTInventoryItemInstance*> GetAllItems() const;

public:
	//~FFastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer contract

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FWitchPTInventoryEntry, FWitchPTInventoryList>(Entries, DeltaParms, *this);
	}

	/**
	 * Create a new inventory entry with the specified item definition and stack count.
	 *
	 * @param ItemClass The class of the inventory item to add.
	 * @param StackCount The number of stacks for the new inventory item.
	 * @return A pointer to the newly created inventory item instance.
	 */
	UWitchPTInventoryItemInstance* AddEntry(TSubclassOf<UWitchPTInventoryItemDefinition> ItemClass, int32 StackCount);
	/**
	 * Adds an item instance to the inventory list.
	 *
	 * @param Instance The inventory item instance to add.
	 */
	void AddEntry(UWitchPTInventoryItemInstance* Instance);

	void RemoveEntry(UWitchPTInventoryItemInstance* Instance);

private:
	void BroadcastChangeMessage(FWitchPTInventoryEntry& Entry, int32 OldCount, int32 NewCount);

private:
	friend UWitchPTInventoryManagerComponent;

private:
	// Replicated list of items
	UPROPERTY()
	TArray<FWitchPTInventoryEntry> Entries;

	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;
};

template<>
struct TStructOpsTypeTraits<FWitchPTInventoryList> : public TStructOpsTypeTraitsBase2<FWitchPTInventoryList>
{
	enum { WithNetDeltaSerializer = true };
};








UCLASS(BlueprintType)
class WITCHPT_API UWitchPTInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWitchPTInventoryManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	bool CanAddItemDefinition(TSubclassOf<UWitchPTInventoryItemDefinition> ItemDef, int32 StackCount = 1);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	UWitchPTInventoryItemInstance* AddItemDefinition(TSubclassOf<UWitchPTInventoryItemDefinition> ItemDef, int32 StackCount = 1);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	void AddItemInstance(UWitchPTInventoryItemInstance* ItemInstance);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	void RemoveItemInstance(UWitchPTInventoryItemInstance* ItemInstance);

	UFUNCTION(BlueprintCallable, Category=Inventory, BlueprintPure=false)
	TArray<UWitchPTInventoryItemInstance*> GetAllItems() const;

	UFUNCTION(BlueprintCallable, Category=Inventory, BlueprintPure)
	UWitchPTInventoryItemInstance* FindFirstItemStackByDefinition(TSubclassOf<UWitchPTInventoryItemDefinition> ItemDef) const;

	int32 GetTotalItemCountByDefinition(TSubclassOf<UWitchPTInventoryItemDefinition> ItemDef) const;
	bool ConsumeItemsByDefinition(TSubclassOf<UWitchPTInventoryItemDefinition> ItemDef, int32 NumToConsume);

	// Delegate
	
	FOnItemChanged OnItemAdded;
	FOnItemChanged OnItemRemoved;

	//~UObject interface
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	virtual void ReadyForReplication() override;
	//~End of UObject interface

private:
	
	UPROPERTY(Replicated)
	FWitchPTInventoryList InventoryList;



		
};
