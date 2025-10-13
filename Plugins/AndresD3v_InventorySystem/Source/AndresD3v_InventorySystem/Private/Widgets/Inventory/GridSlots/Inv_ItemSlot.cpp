// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/GridSlots/Inv_ItemSlot.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UInv_ItemSlot::NativeConstruct()
{
	Super::NativeConstruct();
}

void UInv_ItemSlot::SetInventoryItem(UInv_InventoryItem* Item)
{
	InventoryItem = Item;
}

void UInv_ItemSlot::SetStackCount(int32 Count)
{
	StackCount = Count;
	if (Text_StackCount)
	{
		if (StackCount >= 1)
		{
			Text_StackCount->SetText(FText::AsNumber(StackCount));
			Text_StackCount->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			Text_StackCount->SetText(FText::AsNumber(1));
			Text_StackCount->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UInv_ItemSlot::SetImageBrush(const FSlateBrush& Brush)
{
	if (Image_GridSlot)
	{
		Image_GridSlot->SetBrush(Brush);
	}
}
