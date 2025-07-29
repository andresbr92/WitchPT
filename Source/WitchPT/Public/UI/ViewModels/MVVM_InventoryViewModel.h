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
	
	void AddItem(UWitchPTInventoryItemInstance* ItemInstance);
	void RemoveItem(UWitchPTInventoryItemInstance* ItemInstance);
	
	void SetItems(const TArray<UWitchPTInventoryItemInstance*>& NewItems) { Items = NewItems; }
	TArray<UWitchPTInventoryItemInstance*> GetItems() const;
	
};
