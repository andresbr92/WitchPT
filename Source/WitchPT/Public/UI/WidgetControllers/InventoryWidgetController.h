// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WitchPTWidgetController.h"
#include "InventoryWidgetController.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemAddedSignature, UInventoryItemDefinition*, ItemAdded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemRemovedSignature, UInventoryItemDefinition*, ItemRemoved);

UCLASS(BlueprintType, Blueprintable)
class WITCHPT_API UInventoryWidgetController : public UWitchPTWidgetController
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	virtual void BindCallbacksToDependencies() override;
						
	/**
	 * Removes a stack or specified amount from an inventory item instance
	 * If there's only one stack left and count is 1, removes the item completely
	 * @param ItemInstance - The item instance to remove from
	 * @param AmountToRemove - The amount to remove, defaults to 1
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveItemStack(UInventoryItemDefinition* ItemInstance, int32 AmountToRemove = 1);

	UPROPERTY(BlueprintAssignable, Category="Inventory")
	FOnItemAddedSignature OnItemAddedDelegate;
	UPROPERTY(BlueprintAssignable, Category="Inventory")
	FOnItemAddedSignature OnItemStackChangedDelegate;
	UPROPERTY(BlueprintAssignable, Category="Inventory")
	FOnItemRemovedSignature OnItemRemovedDelegate;
};
