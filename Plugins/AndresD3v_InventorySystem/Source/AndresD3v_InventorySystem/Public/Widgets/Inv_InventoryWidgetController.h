// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/Inv_GridTypes.h"
#include "UObject/Object.h"
#include "Inv_InventoryWidgetController.generated.h"

class UInv_InventoryItem;
class UInv_InventoryComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryListChanged);
/**
 * 
 */
UCLASS()
class ANDRESD3V_INVENTORYSYSTEM_API UInv_InventoryWidgetController : public UObject
{
	GENERATED_BODY()
public:
	void Activate();
	void Deactivate();


	UFUNCTION()
	void SetInventoryComponent(UInv_InventoryComponent* InInventoryComponent);

	UPROPERTY(BlueprintAssignable, Category = "WidgetController")
	FOnInventoryListChanged OnInventoryListChanged;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	TArray<TObjectPtr<UInv_InventoryItem>> InventoryItems;
	
protected:
	virtual void BroadcastInitialValues();
	virtual void BindCallbacksToDependencies();
	virtual void UnbindCallbacksFromDependencies();
	
	UPROPERTY(BlueprintReadOnly, Category= "Inventory")
	TObjectPtr<UInv_InventoryComponent> InventoryComponent;

	
	UFUNCTION()
	void OnItemAdded(UInv_InventoryItem* Item);
	UFUNCTION()
	void OnItemRemoved(UInv_InventoryItem* Item);
	UFUNCTION()
	void OnStackChanged(const FInv_SlotAvailabilityResult& Result);
	
};
