// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetControllers/CauldronWidgetController.h"

#include "Item/CauldronAltar.h"

UCauldronWidgetController::UCauldronWidgetController()
{
}

void UCauldronWidgetController::BroadcastInitialValues()
{
	if(CauldronAltar)
	{
		// OnBaseIngredientDropped.Broadcast(CauldronAltar->GetBaseIngredient());
	}
	
}

void UCauldronWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();
	if(CauldronAltar)
	{
		CauldronAltar->OnBaseIngredientDropped.AddDynamic(this, &UCauldronWidgetController::HandleBaseIngredientDropped);
	}
}

void UCauldronWidgetController::SetCauldronAltar(ACauldronAltar* InCauldronAltar)
{
	if (InCauldronAltar != nullptr)
	{
		CauldronAltar = InCauldronAltar;
	}
	
}

void UCauldronWidgetController::HandleBaseIngredientDropped(UInventoryItemDefinition* IngredientInstance)
{
	OnBaseIngredientDropped.Broadcast(IngredientInstance);
}
