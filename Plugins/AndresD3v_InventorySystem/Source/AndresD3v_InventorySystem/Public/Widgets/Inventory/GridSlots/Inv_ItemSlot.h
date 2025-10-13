// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Items/Inv_InventoryItem.h"
#include "Inv_ItemSlot.generated.h"

class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class ANDRESD3V_INVENTORYSYSTEM_API UInv_ItemSlot : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	TWeakObjectPtr<UInv_InventoryItem> GetInventoryItem() const { return InventoryItem; }
	void SetInventoryItem(UInv_InventoryItem* Item);
	int32 GetStackCount() const { return StackCount; }
	void SetStackCount(int32 Count);
	void SetImageBrush(const FSlateBrush& Brush);
	

private:
	int32 StackCount{0};
	TWeakObjectPtr<UInv_InventoryItem> InventoryItem;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_GridSlot;

	// create the stack text block
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_StackCount;
};
