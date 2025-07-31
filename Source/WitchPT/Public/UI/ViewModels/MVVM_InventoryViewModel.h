// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WitchPT_MVVMViewModelBase.h"
#include "MVVM_InventoryViewModel.generated.h"

class UWitchPTInventoryItemInstance;
/**
 * 
 */
UCLASS()
class WITCHPT_API UMVVM_InventoryViewModel : public UWitchPT_MVVMViewModelBase
{
	GENERATED_BODY()
public:
	virtual void BindCallbacksToDependencies() override;
	virtual void BroadcastInitialValues() override;
private:
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess))
	TArray<UWitchPTInventoryItemInstance*> Items;

public:
	// -------------------- Getters and Setters ------------------------
	void SetItems(const TArray<UWitchPTInventoryItemInstance*>& NewItems) { Items = NewItems; }
	TArray<UWitchPTInventoryItemInstance*> GetItems() const;
	// -------------------- Callbacks ----------------------------------
	void AddItem(UWitchPTInventoryItemInstance* ItemInstance);
	void RemoveItem(UWitchPTInventoryItemInstance* ItemInstance);

	// -------------------- MVVM -> Model Functions --------------------
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void EquipItem(UWitchPTInventoryItemInstance* ItemInstance);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveItemStack(UWitchPTInventoryItemInstance* ItemInstance, int32 AmountToRemove = 1);
	
	
};
