// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpecHandle.h"
#include "Components/ActorComponent.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "WitchPTEquipmentManagerComponent.generated.h"


class UWitchPTEquipmentManagerComponent;
class UWitchPTInventoryItemInstance;
class UAbilitySystemComponent;
class UWitchPTEquipmentDefinition;
class UWitchPTEquipmentInstance;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemEquipped, UWitchPTEquipmentInstance*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemUnequipped, UWitchPTEquipmentInstance*);

USTRUCT()
struct FEquipmentAbilityHandles
{
	GENERATED_BODY()
	TArray<FGameplayAbilitySpecHandle> GrantedHandles;
};

USTRUCT(BlueprintType)
struct FWitchPTEquipmentEntry: public FFastArraySerializerItem
{
	GENERATED_BODY()
	FWitchPTEquipmentEntry() {};

	UPROPERTY()
	TSubclassOf<UWitchPTEquipmentDefinition> EquipmentDefinition;

	UPROPERTY()
	TObjectPtr<UWitchPTEquipmentInstance> Instance = nullptr;

	FEquipmentAbilityHandles AbilityHandles;
};

USTRUCT(BlueprintType)
struct FWitchPTEquipmentList : public FFastArraySerializer
{
	GENERATED_BODY()

	FWitchPTEquipmentList() : OwnerComponent(nullptr) {}
	FWitchPTEquipmentList(UActorComponent* InOwnerComponent) : OwnerComponent(InOwnerComponent) {}

	//~FFastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer contract
	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FWitchPTEquipmentEntry, FWitchPTEquipmentList>(Entries, DeltaParms, *this);
	}

	

	UWitchPTEquipmentInstance* AddEntry(TSubclassOf<UWitchPTEquipmentDefinition> EquipmentDefinition);
	void RemoveEntry(UWitchPTEquipmentInstance* Instance);

private:
	UAbilitySystemComponent* GetAbilitySystemComponent() const;

	UPROPERTY()
	TArray<FWitchPTEquipmentEntry> Entries;

	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;
    
	friend UWitchPTEquipmentManagerComponent;
};

// Add template specialization:
template<>
struct TStructOpsTypeTraits<FWitchPTEquipmentList> : public TStructOpsTypeTraitsBase2<FWitchPTEquipmentList>
{
	enum { WithNetDeltaSerializer = true };
};


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class WITCHPT_API UWitchPTEquipmentManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UWitchPTEquipmentManagerComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	//~UActorComponent interface
	//virtual void EndPlay() override;
	virtual void InitializeComponent() override;
	virtual void UninitializeComponent() override;
	virtual void ReadyForReplication() override;
	//~End of UActorComponent interface

	UFUNCTION(BlueprintCallable, Category="Equipment")
	void EquipItem(TSubclassOf<UWitchPTEquipmentDefinition> EquipmentDefinition);

	UFUNCTION(BlueprintCallable, Category="Equipment")
	void UnequipItem(UWitchPTEquipmentInstance* ItemInstance);

	UFUNCTION(Server, Reliable)
	void Server_EquipItem(TSubclassOf<UWitchPTEquipmentDefinition> EquipmentDefinition);

	UFUNCTION(Server, Reliable)
	void Server_UnequipItem(UWitchPTEquipmentInstance* ItemInstance);
	

	UFUNCTION(BlueprintPure, Category="Equipment")
	bool IsInventoryItemEquipped(UWitchPTInventoryItemInstance* InventoryItem) const;

	UFUNCTION(BlueprintPure, Category="Equipment")
	UWitchPTEquipmentInstance* FindEquipmentByInventoryItem(UWitchPTInventoryItemInstance* InventoryItem) const;
	

	UFUNCTION(BlueprintCallable, Category="Equipment")
	void PrintEquippedItems();

	FOnItemEquipped OnItemEquipped;
	FOnItemUnequipped OnItemUnequipped;

protected:
	UPROPERTY(Replicated)
	FWitchPTEquipmentList EquipmentList;

	// Add after EquippedItems:
	UPROPERTY()
	TMap<TObjectPtr<UWitchPTEquipmentInstance>, FEquipmentAbilityHandles> EquipmentAbilities;

private:
	UAbilitySystemComponent* GetAbilitySystemComponent() const;


};
