// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetControllers/QuickBarWidgetController.h"

#include "Equipment/WitchPTQuickBarComponent.h"
#include "Player/WitchPTPlayerController.h"

void UQuickBarWidgetController::BroadcastInitialValues()
{

}

void UQuickBarWidgetController::BindCallbacksToDependencies()
{
	
	if (AWitchPTPlayerController* WitchPtPlayerController = Cast<AWitchPTPlayerController>(PlayerController))
	{
		
		if (UWitchPTQuickBarComponent* QuickBarComponent = WitchPtPlayerController->GetQuickBarComponent())
		{
			QuickBarComponent->OnActiveSlotChanged.AddDynamic(this, &UQuickBarWidgetController::OnActiveSlotChanged);
		}
	}
	
}

void UQuickBarWidgetController::OnActiveSlotChanged(int32 NewActiveSlotIndex)
{
	OnActiveSlotChangedDelegate.Broadcast(NewActiveSlotIndex);
}
