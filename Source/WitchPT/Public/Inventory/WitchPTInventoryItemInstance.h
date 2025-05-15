// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Systems/GameplayTagStack.h"
#include "Templates/SubclassOf.h"
#include "WitchPTInventoryItemInstance.generated.h"
class UWitchPTInventoryItemFragment;
class UWitchPTInventoryItemDefinition;
struct FFrame;
struct FGameplayTag;

/**
 * 
 */
UCLASS(BlueprintType)
class WITCHPT_API UWitchPTInventoryItemInstance : public UObject
{
	GENERATED_BODY()
public:
	UWitchPTInventoryItemInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	//~UObject interface
	virtual bool IsSupportedForNetworking() const override { return true; }
	//~End of UObject interface

	// // Adds a specified number of stacks to the tag (does nothing if StackCount is below 1)
	// UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	// void AddStatTagStack(FGameplayTag Tag, int32 StackCount);
	//
	// // Removes a specified number of stacks from the tag (does nothing if StackCount is below 1)
	// UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category= Inventory)
	// void RemoveStatTagStack(FGameplayTag Tag, int32 StackCount);
	//
	// // Returns the stack count of the specified tag (or 0 if the tag is not present)
	// UFUNCTION(BlueprintCallable, Category=Inventory)
	// int32 GetStatTagStackCount(FGameplayTag Tag) const;
	//
	// // Returns true if there is at least one stack of the specified tag
	// UFUNCTION(BlueprintCallable, Category=Inventory)
	// bool HasStatTag(FGameplayTag Tag) const;

	int32 GetTotalStackCount() const { return TotalStackCount; }
	void SetTotalStackCount(int32 Count) { TotalStackCount = Count; }

	TSubclassOf<UWitchPTInventoryItemDefinition> GetItemDef() const
	{
		return ItemDef;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure=false, meta=(DeterminesOutputType=FragmentClass))
	const UWitchPTInventoryItemFragment* FindFragmentByClass(TSubclassOf<UWitchPTInventoryItemFragment> FragmentClass) const;

	template <typename ResultClass>
	const ResultClass* FindFragmentByClass() const
	{
		return (ResultClass*)FindFragmentByClass(ResultClass::StaticClass());
	}

	void SetItemDef(TSubclassOf<UWitchPTInventoryItemDefinition> InDef);

	friend struct FWitchPTInventoryList;

private:
	
	UPROPERTY(Replicated)
	FGameplayTagStackContainer StatTags;

	UPROPERTY(Replicated)
	int32 TotalStackCount{0};

	

	
	UPROPERTY(Replicated)
	TSubclassOf<UWitchPTInventoryItemDefinition> ItemDef;
	
};
