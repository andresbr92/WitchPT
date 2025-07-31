// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModels/MVVM_RitualViewModel.h"

#include "Item/RitualAltar.h"

void UMVVM_RitualViewModel::UnbindCallbacksFromDependencies()
{
	
}

void UMVVM_RitualViewModel::BindCallbacksToDependencies()
{
	if (RitualAltar)
	{
		RitualAltar->OnRitualStateChangedEvent.AddDynamic(this, &UMVVM_RitualViewModel::HandleRitualStateChanged);
		RitualAltar->OnReadyPlayersChangedEvent.AddDynamic(this, &UMVVM_RitualViewModel::HandleReadyPlayersChanged);
		RitualAltar->OnCountdownTickEvent.AddDynamic(this, &UMVVM_RitualViewModel::HandleCountdownTick);
		RitualAltar->OnTurnDataChangedEvent.AddDynamic(this, &UMVVM_RitualViewModel::HandleTurnDataChanged);
		RitualAltar->OnCorruptionChangedEvent.AddDynamic(this, &UMVVM_RitualViewModel::HandleCorruptionChanged);
		RitualAltar->OnSequenceProgressChangedEvent.AddDynamic(this, &UMVVM_RitualViewModel::HandleSequenceProgressChanged);
		RitualAltar->OnRitualCompletedEvent.AddDynamic(this, &UMVVM_RitualViewModel::HandleRitualCompleted);
	}
	
}

void UMVVM_RitualViewModel::BroadcastInitialValues()
{
	if (RitualAltar)
	{
		SetCurrentRitualState(RitualAltar->GetCurrentRitualState());
		SetProgressPercentage(RitualAltar->GetCurrentSequenceProgress());
		SetCorruptionPercentage(RitualAltar->GetCorruptionPercentage());
		FUIRitualData CurrentTurnData = RitualAltar->GetCurrentTurnData();
		SetCurrentInputTimeRemaining(CurrentTurnData.CurrentInputTimeRemaining);
		
	}
	
}

void UMVVM_RitualViewModel::SetCurrentRitualState(EInteractionState NewState)
{
	UE_MVVM_SET_PROPERTY_VALUE(CurrentRitualState, NewState);
}

void UMVVM_RitualViewModel::SetExpectedInput(const FString& NewExpectedInput)
{
	UE_MVVM_SET_PROPERTY_VALUE(ExpectedInput, NewExpectedInput);
}

void UMVVM_RitualViewModel::SetRitualAltar(ARitualAltar* InRitualAltar)
{
	if (InRitualAltar)
	{
		RitualAltar = InRitualAltar;
		
		UnbindCallbacksFromDependencies();
		BindCallbacksToDependencies();
		BroadcastInitialValues();
	}
}

void UMVVM_RitualViewModel::HandleRitualStateChanged(const EInteractionState NewState)
{
	UE_MVVM_SET_PROPERTY_VALUE(CurrentRitualState, NewState);
}

void UMVVM_RitualViewModel::HandleTurnDataChanged(FUIRitualData TurnData)
{
	
	UE_MVVM_SET_PROPERTY_VALUE(ProgressPercentage, TurnData.RitualPercentageCompleted);
	UE_MVVM_SET_PROPERTY_VALUE(CorruptionPercentage, TurnData.CorruptionPercentage);
	UE_MVVM_SET_PROPERTY_VALUE(CurrentInputTimeRemaining, TurnData.CurrentInputTimeRemaining);
	if (TurnData.bIsMyTurn)
	{
		UE_MVVM_SET_PROPERTY_VALUE(ExpectedInput, TurnData.ExpectedInput.ToString());
	} else 
	{
		UE_MVVM_SET_PROPERTY_VALUE(ExpectedInput, FGameplayTag::EmptyTag.ToString());
	}
}

void UMVVM_RitualViewModel::HandleReadyPlayersChanged(FRitualReadyPlayersData InReadyPlayersData)
{
	
	
}

void UMVVM_RitualViewModel::HandleCountdownTick(int32 InCountdownValue)
{
	if (InCountdownValue <= 0)
	{
		SetCurrentInputTimeRemaining(0.0f);
	}
	else
	{
		SetCurrentInputTimeRemaining(static_cast<float>(InCountdownValue));
	}
	
}

void UMVVM_RitualViewModel::HandleCorruptionChanged(float InCorruptionPercentage)
{
	UE_MVVM_SET_PROPERTY_VALUE(CorruptionPercentage, InCorruptionPercentage);
}

void UMVVM_RitualViewModel::HandleSequenceProgressChanged(float InProgressPercentage)
{
	UE_MVVM_SET_PROPERTY_VALUE(ProgressPercentage, InProgressPercentage);
}

void UMVVM_RitualViewModel::HandleRitualCompleted(bool bWasSuccessful)
{
	
}
