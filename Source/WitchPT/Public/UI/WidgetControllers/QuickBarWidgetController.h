// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WitchPTWidgetController.h"
#include "QuickBarWidgetController.generated.h"

class UWitchPTInventoryItemInstance;
class UWitchPTQuickBarComponent;
/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuickBarSlotChangedSignature, int32, NewActiveSlotIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEquipableItemDragStartSignature);
UCLASS(BlueprintType, Blueprintable)
class WITCHPT_API UQuickBarWidgetController : public UWitchPTWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;


	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void AddItemToQuickBarSlot(int32 SlotIndex, UWitchPTInventoryItemInstance* ItemInstance);

	// --------------------------- DELEGATES ---------------------------
	UPROPERTY(BlueprintAssignable, Category = "QuickBar")
	FOnQuickBarSlotChangedSignature OnActiveSlotChangedDelegate;
	UPROPERTY(BlueprintAssignable, Category = "QuickBar")
	FOnEquipableItemDragStartSignature OnEquipableItemDragStartDelegate;


protected:
	UFUNCTION()
	void OnActiveSlotChanged(int32 NewActiveSlotIndex);


private:
	UFUNCTION()
	void OnEquipableItemDragStart(const UWitchPTInventoryItemInstance* ItemInstance);
	
	
	
};
