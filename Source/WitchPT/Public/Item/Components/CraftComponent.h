// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameplayTagContainer.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "CraftComponent.generated.h"

class APotionBase;
class UCraftComponent;
class UWitchPTInventoryItemDefinition;
class UWitchPTInventoryItemInstance;
class ACharacter;

USTRUCT(BlueprintType)
struct FIngredientSetPayload
{
	GENERATED_BODY()

	
	UPROPERTY(BlueprintReadOnly, Category = "Payload")
	TObjectPtr<UWitchPTInventoryItemInstance> ItemInstance;


};

USTRUCT(BlueprintType)
struct FCauldronIngredientEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FCauldronIngredientEntry() {}

	UPROPERTY()
	TObjectPtr<UWitchPTInventoryItemInstance> IngredientInstance = nullptr;
	UPROPERTY()
	int32 SlotIndex = INDEX_NONE;
};
USTRUCT(BlueprintType)
struct FCauldronIngredientList : public FFastArraySerializer
{
	GENERATED_BODY()

	FCauldronIngredientList() : OwnerComponent(nullptr) {}
	FCauldronIngredientList(UActorComponent* InOwnerComponent)	: OwnerComponent(InOwnerComponent)
	{
	}

	// FastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	void NotifyIngredientChanged(int32 SlotIndex, UWitchPTInventoryItemInstance* Instance);

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FCauldronIngredientEntry, FCauldronIngredientList>(Entries, DeltaParms, *this);
	}

	// Helper functions
	void SetIngredientAtSlot(int32 SlotIndex, UWitchPTInventoryItemInstance* Instance);
	UWitchPTInventoryItemInstance* GetIngredientAtSlot(int32 SlotIndex) const;

public:
	UPROPERTY()
	TArray<FCauldronIngredientEntry> Entries;

private:
	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;

	friend UCraftComponent;
};

/**
 * Trait specification for FCauldronIngredientList
 * Enables network delta serialization for efficient replication
 */
template<>
struct TStructOpsTypeTraits<FCauldronIngredientList> : public TStructOpsTypeTraitsBase2<FCauldronIngredientList>
{
	enum { WithNetDeltaSerializer = true };
};


UCLASS(Blueprintable)
class WITCHPT_API UCraftComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCraftComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	// Replication
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	virtual void ReadyForReplication() override;
	
	UPROPERTY(Replicated, VisibleAnywhere)
	FCauldronIngredientList IngredientList;
	UPROPERTY(EditDefaultsOnly, Category="Config")
	TSubclassOf<UWitchPTInventoryItemDefinition> PotionItemDefinitionClass;
	UPROPERTY(EditDefaultsOnly, Category="Config")
	TSubclassOf<APotionBase> PotionActorClass;

	void TryAddIngredient(const ACharacter* RequestingCharacter, UWitchPTInventoryItemInstance* IngredientInstance);
	void CraftPotion(const ACharacter* RequestingCharacter) const;
	
	void GetIngredientAtSlot(int32 SlotIndex, UWitchPTInventoryItemInstance*& OutIngredientInstance) const;
	

};
