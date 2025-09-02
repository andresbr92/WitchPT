// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameplayTagContainer.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "CauldronCraftComponent.generated.h"

class UCauldronCraftComponent;
class UWitchPTInventoryItemDefinition;
class UWitchPTInventoryItemInstance;
class ACharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBaseIngredientSetSignature, UWitchPTInventoryItemInstance*, IngredientInstance);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPrincipalIngredientSetSignature, UWitchPTInventoryItemInstance*, IngredientInstance);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnModifierIngredientSetSignature, UWitchPTInventoryItemInstance*, IngredientInstance);
USTRUCT(BlueprintType)
struct FIngredientSetPayload
{
	GENERATED_BODY()

	
	UPROPERTY(BlueprintReadOnly, Category = "Payload")
	TSubclassOf<UWitchPTInventoryItemDefinition> ItemInstance;


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

private:
	UPROPERTY()
	TArray<FCauldronIngredientEntry> Entries;

	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;

	friend UCauldronCraftComponent;
};


UCLASS(Blueprintable)
class WITCHPT_API UCauldronCraftComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCauldronCraftComponent();

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




	// Delegates
	FOnBaseIngredientSetSignature OnBaseIngredientSetDelegate;
	FOnPrincipalIngredientSetSignature OnPrincipalIngredientSetDelegate;
	FOnModifierIngredientSetSignature OnModifierIngredientSetDelegate;


	// Ingredient Functions
	
	void TrySetIngredientInSlot(const ACharacter* RequestingCharacter, const TSubclassOf<UWitchPTInventoryItemDefinition>& IngredientItemDef);
	
	void GetIngredientAtSlot(int32 SlotIndex, UWitchPTInventoryItemInstance*& OutIngredientInstance) const;

protected:
	// Broadcast Helper Functions
	void BroadcastBaseIngredientDropped() const;
	void BroadcastBaseIngredientIconSet() const;
	void BroadcastPrincipalIngredientDropped() const;
	void BroadcastPrincipalIngredientIconSet() const;
	void BroadcastModifierIngredientDropped() const;
	void BroadcastModifierIngredientIconSet() const;

private:

};
