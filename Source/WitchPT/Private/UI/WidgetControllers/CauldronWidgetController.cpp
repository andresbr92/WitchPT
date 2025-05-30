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
		CauldronAltar->OnPrincipalIngredientSetDelegate.AddDynamic(this, &UCauldronWidgetController::HandlePrincipalIngredientSet);
		CauldronAltar->OnPrincipalIngredientIconSetDelegate.AddDynamic(this, &UCauldronWidgetController::HandlePrincipalIngredientIconSet);
		CauldronAltar->OnModifierIngredientSetDelegate.AddDynamic(this, &UCauldronWidgetController::HandleModifierIngredientSet);
		CauldronAltar->OnModifierIngredientIconSetDelegate.AddDynamic(this, &UCauldronWidgetController::HandleModifierIngredientIconSet);
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

void UCauldronWidgetController::HandlePrincipalIngredientSet(UWitchPTInventoryItemInstance* IngredientInstance)
{
	OnPrincipalIngredientSet.Broadcast(IngredientInstance);
}

void UCauldronWidgetController::HandlePrincipalIngredientIconSet(TSubclassOf<UUserWidget> PrincipalIngredientIcon)
{
	OnPrincipalIngredientIconSet.Broadcast(PrincipalIngredientIcon);
}

void UCauldronWidgetController::HandleModifierIngredientSet(UWitchPTInventoryItemInstance* IngredientInstance)
{
	OnModifierIngredientSet.Broadcast(IngredientInstance);
}

void UCauldronWidgetController::HandleModifierIngredientIconSet(TSubclassOf<UUserWidget> ModifierIngredientIcon)
{
	OnModifierIngredientIconSet.Broadcast(ModifierIngredientIcon);
}
