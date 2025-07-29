// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModels/MVVM_QuickBarViewModel.h"

#include "Equipment/WitchPTQuickBarComponent.h"
#include "Player/WitchPTPlayerController.h"

void UMVVM_QuickBarViewModel::BindCallbacksToDependencies()
{
	if (AWitchPTPlayerController* WitchPtPlayerController = Cast<AWitchPTPlayerController>(PlayerController))
	{
		
		if (UWitchPTQuickBarComponent* QuickBarComponent = WitchPtPlayerController->GetQuickBarComponent())
		{
			QuickBarComponent->OnActiveSlotChanged.AddLambda([this](const int32 NewSlotIndex)
			{
				SetActiveSlotIndex(NewSlotIndex);
			});
		}
	}
	
}

void UMVVM_QuickBarViewModel::BroadcastInitialValues()
{
	if (AWitchPTPlayerController* WitchPtPlayerController = Cast<AWitchPTPlayerController>(PlayerController))
	{
		if (UWitchPTQuickBarComponent* QuickBarComponent = WitchPtPlayerController->GetQuickBarComponent())
		{
			SetActiveSlotIndex(QuickBarComponent->GetActiveSlotIndex());
		}
	}
}

void UMVVM_QuickBarViewModel::SetActiveSlotIndex(int32 NewActiveSlotIndex)
{
	UE_MVVM_SET_PROPERTY_VALUE(ActiveSlotIndex, NewActiveSlotIndex);
}
