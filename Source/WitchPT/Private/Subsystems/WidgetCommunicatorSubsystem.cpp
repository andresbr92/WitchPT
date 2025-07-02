// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/WidgetCommunicatorSubsystem.h"

void UWidgetCommunicatorSubsystem::OnItemDragged(UWitchPTInventoryItemInstance* ItemInstance)
{
    OnItemDraggedDelegate.Broadcast(ItemInstance);
}
