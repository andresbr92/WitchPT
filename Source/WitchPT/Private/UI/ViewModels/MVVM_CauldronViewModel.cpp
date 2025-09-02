// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModels/MVVM_CauldronViewModel.h"

#include "GameFramework/Character.h"
#include "Item/CauldronAltar.h"
#include "Item/Components/CauldronCraftComponent.h"
#include "Subsystems/UIManagerSubsystem.h"
#include "UI/GameUIContexts/CauldronUIContext.h"
#include "UI/GameUIContexts/WitchPT_GameUIContextBase.h"

void UMVVM_CauldronViewModel::BindCallbacksToDependencies()
{
	if (CauldronAltar && CauldronAltar->CauldronCraftComponent)
	{
		CauldronAltar->CauldronCraftComponent->OnBaseIngredientSetDelegate.AddDynamic(this, &UMVVM_CauldronViewModel::SetBaseIngredient);
		CauldronAltar->CauldronCraftComponent->OnPrincipalIngredientSetDelegate.AddDynamic(this, &UMVVM_CauldronViewModel::SetPrincipalIngredient);
		CauldronAltar->CauldronCraftComponent->OnModifierIngredientSetDelegate.AddDynamic(this, &UMVVM_CauldronViewModel::SetModifierIngredient);
		
		
	}
}

void UMVVM_CauldronViewModel::BroadcastInitialValues()
{
	if (CauldronAltar && CauldronAltar->CauldronCraftComponent)
	{
		SetBaseIngredient(CauldronAltar->CauldronCraftComponent->IngredientList.GetIngredientAtSlot(0));
		SetPrincipalIngredient(CauldronAltar->CauldronCraftComponent->IngredientList.GetIngredientAtSlot(1));
		SetModifierIngredient(CauldronAltar->CauldronCraftComponent->IngredientList.GetIngredientAtSlot(2));
	
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

void UMVVM_CauldronViewModel::SetPrincipalIngredient(UWitchPTInventoryItemInstance* InPrincipalIngredient)
{
	UE_MVVM_SET_PROPERTY_VALUE(PrincipalIngredient, InPrincipalIngredient);
}

void UMVVM_CauldronViewModel::SetModifierIngredient(UWitchPTInventoryItemInstance* InModifierIngredient)
{
	UE_MVVM_SET_PROPERTY_VALUE(ModifierIngredient, InModifierIngredient);
}

void UMVVM_CauldronViewModel::TrySetIngredientInSlot(const ACharacter* RequestingCharacter,
	const TSubclassOf<UWitchPTInventoryItemDefinition>& IngredientItemDef)
{
	if (!CauldronAltar)
	{
		if (UUIManagerSubsystem* UIManagerSubsystem =GetWorld()->GetGameInstance()->GetSubsystem<UUIManagerSubsystem>())
		{
			UWitchPT_GameUIContextBase* GameUIContext = nullptr;
			APlayerController* InPlayerController = RequestingCharacter ? RequestingCharacter->GetController<APlayerController>() : nullptr;
			ULocalPlayer* InLocalPlayer = InPlayerController ? Cast<ULocalPlayer>(InPlayerController->GetLocalPlayer()) : nullptr;
			UIManagerSubsystem->FindUIContext_ForPlayer(InLocalPlayer, UCauldronUIContext::StaticClass(), GameUIContext);
			if (GameUIContext)
			{
				if (UCauldronUIContext* CauldronUIContext = Cast<UCauldronUIContext>(GameUIContext))
				{
					CauldronAltar = CauldronUIContext->CauldronAltar;
				}
			}
		}
		
	}
	if (CauldronAltar && CauldronAltar->CauldronCraftComponent)
	{
		CauldronAltar->CauldronCraftComponent->TrySetIngredientInSlot(RequestingCharacter, IngredientItemDef);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UMVVM_CauldronViewModel::TrySetIngredientInSlot: CauldronCraftComponent is not valid."));
	}
}
