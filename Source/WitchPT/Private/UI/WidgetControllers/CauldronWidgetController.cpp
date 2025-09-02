// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetControllers/CauldronWidgetController.h"

#include "Item/CauldronAltar.h"
#include "Item/Components/CauldronCraftComponent.h"
#include "Kismet/GameplayStatics.h"

void UCauldronWidgetController::SetContextualObject(UObject* ContextualObject)
{
	if (ContextualObject)
	{
		if (ACauldronAltar* InCauldronAltar = Cast<ACauldronAltar>(ContextualObject))
		{
			CauldronAltar = InCauldronAltar;
		}
	}
}

UCauldronWidgetController::UCauldronWidgetController()
{
	
}

void UCauldronWidgetController::BroadcastInitialValues()
{
	// find the CauldronAltar in the world
	CauldronAltar = nullptr; // Initialize to null
	if (GetWorld())
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACauldronAltar::StaticClass(), FoundActors);
		if (FoundActors.Num() > 0)
		{
			CauldronAltar = Cast<ACauldronAltar>(FoundActors[0]);
		}
	}
	if(CauldronAltar)
	{
		// OnBaseIngredientSet.Broadcast(CauldronAltar->CauldronCraftComponent->GetBaseIngredient());
		//
		// OnPrincipalIngredientSet.Broadcast(CauldronAltar->CauldronCraftComponent->GetPrincipalIngredient());
		//
		// OnModifierIngredientSet.Broadcast(CauldronAltar->CauldronCraftComponent->GetModifierIngredient());
		
	}
	
}

void UCauldronWidgetController::BindCallbacksToDependencies()
{
	if(CauldronAltar && CauldronAltar->CauldronCraftComponent)
	{
		// CauldronAltar->CauldronCraftComponent->OnBaseIngredientSetDelegate.AddDynamic(this, &UCauldronWidgetController::HandleBaseIngredientSet);
		// CauldronAltar->CauldronCraftComponent->OnBaseIngredientIconSetDelegate.AddDynamic(this, &UCauldronWidgetController::HandleBaseIngredientIconSet);
		// CauldronAltar->CauldronCraftComponent->OnPrincipalIngredientSetDelegate.AddDynamic(this, &UCauldronWidgetController::HandlePrincipalIngredientSet);
		// CauldronAltar->CauldronCraftComponent->OnPrincipalIngredientIconSetDelegate.AddDynamic(this, &UCauldronWidgetController::HandlePrincipalIngredientIconSet);
		// CauldronAltar->CauldronCraftComponent->OnModifierIngredientSetDelegate.AddDynamic(this, &UCauldronWidgetController::HandleModifierIngredientSet);
		// CauldronAltar->CauldronCraftComponent->OnModifierIngredientIconSetDelegate.AddDynamic(this, &UCauldronWidgetController::HandleModifierIngredientIconSet);
	}
}

void UCauldronWidgetController::UnbindCallbacksFromDependencies()
{
	if(CauldronAltar && CauldronAltar->CauldronCraftComponent)
	{
		// CauldronAltar->CauldronCraftComponent->OnBaseIngredientSetDelegate.RemoveDynamic(this, &UCauldronWidgetController::HandleBaseIngredientSet);
		// CauldronAltar->CauldronCraftComponent->OnBaseIngredientIconSetDelegate.RemoveDynamic(this, &UCauldronWidgetController::HandleBaseIngredientIconSet);
		// CauldronAltar->CauldronCraftComponent->OnPrincipalIngredientSetDelegate.RemoveDynamic(this, &UCauldronWidgetController::HandlePrincipalIngredientSet);
		// CauldronAltar->CauldronCraftComponent->OnPrincipalIngredientIconSetDelegate.RemoveDynamic(this, &UCauldronWidgetController::HandlePrincipalIngredientIconSet);
		// CauldronAltar->CauldronCraftComponent->OnModifierIngredientSetDelegate.RemoveDynamic(this, &UCauldronWidgetController::HandleModifierIngredientSet);
		// CauldronAltar->CauldronCraftComponent->OnModifierIngredientIconSetDelegate.RemoveDynamic(this, &UCauldronWidgetController::HandleModifierIngredientIconSet);
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
