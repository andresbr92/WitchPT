// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModels/MVVM_CauldronViewModel.h"

#include "Item/CauldronAltar.h"
#include "Item/Components/CauldronCraftComponent.h"

void UMVVM_CauldronViewModel::BindCallbacksToDependencies()
{
	if (CauldronAltar && CauldronAltar->CauldronCraftComponent)
	{
		CauldronAltar->CauldronCraftComponent->OnBaseIngredientIconSetDelegate.AddDynamic(this, &UMVVM_CauldronViewModel::SetBaseIngredientIcon);
		CauldronAltar->CauldronCraftComponent->OnPrincipalIngredientIconSetDelegate.AddDynamic(this, &UMVVM_CauldronViewModel::SetPrincipalIngredientIcon);
		CauldronAltar->CauldronCraftComponent->OnModifierIngredientIconSetDelegate.AddDynamic(this, &UMVVM_CauldronViewModel::SetModifierIngredientIcon);
		
	}
}

void UMVVM_CauldronViewModel::BroadcastInitialValues()
{
	if (CauldronAltar && CauldronAltar->CauldronCraftComponent)
	{
		SetBaseIngredientIcon(CauldronAltar->CauldronCraftComponent->GetBaseIngredientIcon());
		SetPrincipalIngredientIcon(CauldronAltar->CauldronCraftComponent->GetPrincipalIngredientIcon());
		SetModifierIngredientIcon(CauldronAltar->CauldronCraftComponent->GetModifierIngredientIcon());
	}
	

}

void UMVVM_CauldronViewModel::UnbindCallbacksFromDependencies()
{

}

void UMVVM_CauldronViewModel::SetCauldronAltar(ACauldronAltar* InCauldronAltar)
{
	if (InCauldronAltar)
	{
		CauldronAltar = InCauldronAltar;
		BindCallbacksToDependencies();
		BroadcastInitialValues();
	}
}

void UMVVM_CauldronViewModel::SetBaseIngredient(UWitchPTInventoryItemInstance* InBaseIngredient)
{
	if (InBaseIngredient)
	{
		UE_MVVM_SET_PROPERTY_VALUE(BaseIngredient, InBaseIngredient);
	}
}

void UMVVM_CauldronViewModel::SetBaseIngredientIcon(TSubclassOf<UUserWidget> InBaseIngredientIcon)
{
	UE_MVVM_SET_PROPERTY_VALUE(BaseIngredientIcon, InBaseIngredientIcon);
}

void UMVVM_CauldronViewModel::SetPrincipalIngredientIcon(TSubclassOf<UUserWidget> InPrincipalIngredientIcon)
{
	UE_MVVM_SET_PROPERTY_VALUE(PrincipalIngredientIcon, InPrincipalIngredientIcon);
}

void UMVVM_CauldronViewModel::SetModifierIngredientIcon(TSubclassOf<UUserWidget> InModifierIngredientIcon)
{
	UE_MVVM_SET_PROPERTY_VALUE(ModifierIngredientIcon, InModifierIngredientIcon);
}
