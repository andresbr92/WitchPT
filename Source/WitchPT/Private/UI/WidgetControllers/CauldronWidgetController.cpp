// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetControllers/CauldronWidgetController.h"

#include "Item/CauldronAltar.h"
#include "Item/Components/CauldronCraftComponent.h"

UCauldronWidgetController::UCauldronWidgetController()
{
}

void UCauldronWidgetController::BroadcastInitialValues()
{
	if(CauldronAltar)
	{
		OnBaseIngredientSet.Broadcast(CauldronAltar->CauldronCraftComponent->GetBaseIngredient());
		// OnBaseIngredientIconSet.Broadcast(CauldronAltar->CauldronCraftComponent->GetBaseIngredientIcon());
		OnPrincipalIngredientSet.Broadcast(CauldronAltar->CauldronCraftComponent->GetPrincipalIngredient());
		// OnPrincipalIngredientIconSet.Broadcast(CauldronAltar->CauldronCraftComponent->GetPrincipalIngredientIcon());
		OnModifierIngredientSet.Broadcast(CauldronAltar->CauldronCraftComponent->GetModifierIngredient());
		// OnModifierIngredientIconSet.Broadcast(CauldronAltar->CauldronCraftComponent->GetModifierIngredientIcon());
	}
	
}

void UCauldronWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();
	if(CauldronAltar)
	{
		CauldronAltar->CauldronCraftComponent->OnBaseIngredientSetDelegate.AddDynamic(this, &UCauldronWidgetController::HandleBaseIngredientSet);
		CauldronAltar->CauldronCraftComponent->OnBaseIngredientIconSetDelegate.AddDynamic(this, &UCauldronWidgetController::HandleBaseIngredientIconSet);
		CauldronAltar->CauldronCraftComponent->OnPrincipalIngredientSetDelegate.AddDynamic(this, &UCauldronWidgetController::HandlePrincipalIngredientSet);
		CauldronAltar->CauldronCraftComponent->OnPrincipalIngredientIconSetDelegate.AddDynamic(this, &UCauldronWidgetController::HandlePrincipalIngredientIconSet);
		CauldronAltar->CauldronCraftComponent->OnModifierIngredientSetDelegate.AddDynamic(this, &UCauldronWidgetController::HandleModifierIngredientSet);
		CauldronAltar->CauldronCraftComponent->OnModifierIngredientIconSetDelegate.AddDynamic(this, &UCauldronWidgetController::HandleModifierIngredientIconSet);
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
