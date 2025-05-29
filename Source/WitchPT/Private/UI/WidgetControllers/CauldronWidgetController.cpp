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
		OnBaseIngredientSet.Broadcast(CauldronAltar->GetBaseIngredient());
		
	}
	
}

void UCauldronWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();
	if(CauldronAltar)
	{
		CauldronAltar->OnBaseIngredientSetDelegate.AddDynamic(this, &UCauldronWidgetController::HandleBaseIngredientSet);
		CauldronAltar->OnBaseIngredientIconSetDelegate.AddDynamic(this, &UCauldronWidgetController::HandleBaseIngredientIconSet);
	}
}

void UCauldronWidgetController::SetCauldronAltar(ACauldronAltar* InCauldronAltar)
{
	if (InCauldronAltar != nullptr)
	{
		CauldronAltar = InCauldronAltar;
	}
	
}

void UCauldronWidgetController::HandleBaseIngredientSet(UWitchPTInventoryItemInstance* IngredientInstance)
{
	OnBaseIngredientSet.Broadcast(IngredientInstance);
}

void UCauldronWidgetController::HandleBaseIngredientIconSet(TSubclassOf<UUserWidget> BaseIngredientIcon)
{
	OnBaseIngredientIconSet.Broadcast(BaseIngredientIcon);
}
