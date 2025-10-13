// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/Inv_InventoryList.h"

#include "Components/GridPanel.h"
#include "InventoryManagement/Components/Inv_InventoryComponent.h"
#include "Items/Inv_InventoryItem.h"
#include "Items/Fragments/Inv_FragmentTags.h"
#include "Items/Fragments/Inv_ItemFragment.h"
#include "Widgets/Inventory/GridSlots/Inv_ItemSlot.h"
#include "Widgets/Inv_InventoryWidgetController.h"

void UInv_InventoryList::NativeConstruct()
{
	Super::NativeConstruct();

	// Get the player controller
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (UInv_InventoryComponent* InventoryComp = PC->FindComponentByClass<UInv_InventoryComponent>())
		{
			WidgetController = InventoryComp->WidgetController;
			if (WidgetController)
			{
				WidgetController->OnInventoryListChanged.AddDynamic(this, &ThisClass::OnInventoryListChanged);
				OnInventoryListChanged();
			}
		}
	}
}

void UInv_InventoryList::SetViewModel(UMVVMViewModelBase* InViewModel)
{
	if (InViewModel)
	{
		ViewModel = InViewModel;
	}
}

void UInv_InventoryList::OnInventoryListChanged()
{
	if (!WidgetController || !GridPanel || !ItemWidgetClass) return;


	GridPanel->ClearChildren();
	
	const TArray<UInv_InventoryItem*>& ItemsToShow = WidgetController->InventoryItems;

	// 3. Repoblamos el grid con los nuevos widgets
	for (int32 i = 0; i < ItemsToShow.Num(); ++i)
	{
		if (UInv_InventoryItem* CurrentItem = ItemsToShow[i])
		{
			if (UInv_ItemSlot* NewItemWidget = CreateWidget<UInv_ItemSlot>(this, ItemWidgetClass))
			{
				const int32 Row = i / Columns;
				const int32 Col = i % Columns;
				NewItemWidget->SetInventoryItem(CurrentItem);
				const FInv_ImageFragment* ImageFragment = GetFragment<FInv_ImageFragment>(CurrentItem, FragmentTags::IconFragment);
				FSlateBrush IconBrush;
				IconBrush.SetResourceObject(ImageFragment->GetIcon());
				IconBrush.DrawAs = ESlateBrushDrawType::Image;
				IconBrush.ImageSize = FVector2D(ImageSize);
				NewItemWidget->SetImageBrush(IconBrush);

				const FInv_StackableFragment* StackableFragment = GetFragment<FInv_StackableFragment>(CurrentItem, FragmentTags::StackableFragment);
				if (StackableFragment)
				{
					NewItemWidget->SetStackCount(CurrentItem->GetTotalStackCount() + StackableFragment->GetStackCount());
				}
				
				GridPanel->AddChildToGrid(NewItemWidget, Row, Col);
			}
		}
	}
}
