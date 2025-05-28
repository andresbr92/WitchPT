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
class WITCHPT_API UInventoryItemDefinition : public UObject
{
	GENERATED_BODY()
public:
	UInventoryItemDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	friend struct FWitchPTInventoryList;
	
	//~UObject interface
	virtual bool IsSupportedForNetworking() const override { return true; }
	//~End of UObject interface


	UFUNCTION(BlueprintCallable)
	int32 GetTotalStackCount() const { return TotalStackCount; }
	
	void SetTotalStackCount(int32 Count) { TotalStackCount = Count; }
	UFUNCTION(BlueprintCallable)
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


private:
	UPROPERTY(Replicated)
	int32 TotalStackCount{0};
	
	UPROPERTY(Replicated)
	TSubclassOf<UWitchPTInventoryItemDefinition> ItemDef;
	
};
