// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/WidgetControllers/RitualWidgetController.h"
#include "Item/RitualAltar.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "FWitchPTGameplayTags.h"

URitualWidgetController::URitualWidgetController()
{
    // Constructor
}

void URitualWidgetController::BroadcastInitialValues()
{
    // If we have an altar, broadcast its initial values
    if (RitualAltar)
    {
        // Broadcast current state
        EInteractionState CurrentState = RitualAltar->GetCurrentRitualState();
        
        OnRitualStateChanged.Broadcast(RitualAltar->GetCurrentRitualState());

        OnReadyPlayersNumberChangedSignature.Broadcast(RitualAltar->GetNumberOfTotalPlayers(), RitualAltar->GetNumberOfReadyPlayers());
        
        
        // // Broadcast active player
        // OnRitualActivePlayerChanged.Broadcast(RitualAltar->GetCurrentActivePlayer());
        //
        // // Broadcast expected input
        // OnRitualExpectedInputChanged.Broadcast(RitualAltar->GetCurrentExpectedInput());
        //
        // // Broadcast remaining time
        // OnRitualInputTimerChanged.Broadcast(RitualAltar->GetCurrentInputTimeRemaining());
        //
        // // Broadcast corruption
        // OnRitualCorruptionChanged.Broadcast(RitualAltar->GetCorruptionPercentage());
        //
        // // Broadcast sequence progress
        // OnRitualSequenceProgressChanged.Broadcast(RitualAltar->GetCurrentSequenceProgress());
    }
}

void URitualWidgetController::BindCallbacksToDependencies()
{
    // Bind to altar delegations if it exists
    if (RitualAltar)
    {
        RitualAltar->OnRitualStateChangedDelegate.BindUObject(this, &URitualWidgetController::HandleRitualStateChanged);
        
        
        RitualAltar->OnNumberOfReadyPlayersHasChangedDelegate.BindLambda([this](int32 TotalPlayers, int32 PlayersReady)
        {
            OnReadyPlayersNumberChangedSignature.Broadcast(TotalPlayers, PlayersReady);
        });
        RitualAltar->OnRitualCountdownTickDelegate.BindLambda([this](int32 CountdownValue)
        {
            // todo: assign this to the RitualAltar useWidget
            OnRitualCountdownTickDelegate.Broadcast(CountdownValue);
        });
        
        RitualAltar->OnCurrentActivePlayerChangedDelegate.BindUObject(this, &URitualWidgetController::HandleActivePlayerChanged);

        

        
       
        
    }
}

void URitualWidgetController::SetRitualAltar(ARitualAltar* InRitualAltar)
{
    // Unbind any existing callbacks first
    if (RitualAltar != nullptr)
    {
        // RitualAltar->OnRitualCompleted.RemoveAll(this);
    }
    
    // Assign the new altar
    RitualAltar = InRitualAltar;
    
    // Rebind and broadcast
    // BroadcastInitialValues();
    // BindCallbacksToDependencies();
}

bool URitualWidgetController::IsLocalPlayerActive() const
{
    if (!RitualAltar || !PlayerController)
    {
        return false;
    }
    
    // Get the local character
    ACharacter* LocalCharacter = Cast<ACharacter>(PlayerController->GetPawn());
    if (!LocalCharacter)
    {
        return false;
    }
    
    // Check if this character is the active player
    return RitualAltar->GetCurrentActivePlayer() == LocalCharacter;
}

void URitualWidgetController::HandleRitualStateChanged(EInteractionState NewState)
{
    OnRitualStateChanged.Broadcast(NewState);
}

void URitualWidgetController::HandleActivePlayerChanged(const ACharacter* NewActivePlayer) const
{
    
    OnRitualActivePlayerChangedDelegate.Broadcast(NewActivePlayer);
    
   
}

void URitualWidgetController::HandleInputTimerChanged(float NewTime)
{
    OnRitualInputTimerChanged.Broadcast(NewTime);
}

void URitualWidgetController::HandleCorruptionChanged(float NewCorruption)
{
    OnRitualCorruptionChanged.Broadcast(NewCorruption);
}

void URitualWidgetController::HandleSequenceIndexChanged(int32 NewIndex)
{
    // If we have a valid altar, we can calculate and send the progress
    if (RitualAltar)
    {
        int32 Progress = RitualAltar->GetCurrentSequenceProgress();
        OnRitualSequenceProgressChanged.Broadcast(Progress);
        
        // Also update the expected input
        FGameplayTag ExpectedInput = RitualAltar->GetCurrentExpectedInput();
        OnRitualExpectedInputChanged.Broadcast(ExpectedInput);
    }
}

void URitualWidgetController::HandleRitualCompleted(bool bWasSuccessful)
{
    OnRitualCompleted.Broadcast(bWasSuccessful);
}

void URitualWidgetController::HandleNumberOfReadyPlayersChanged(int32 TotalPlayers, int32 PlayersReady)
{
    OnReadyPlayersNumberChangedSignature.Broadcast(TotalPlayers, PlayersReady);
}


void URitualWidgetController::HandleTurnAdvanced(FGameplayTag Tag, int32 NewCount)
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Turn Advanced!"));
    // // Only process if the tag was added
    // if (NewCount <= 0 || !RitualAltar)
    // {
    //     return;
    // }
    //
    // // Update UI with current values
    // HandleActivePlayerChanged(RitualAltar->GetCurrentActivePlayer());
    // HandleInputTimerChanged(RitualAltar->GetCurrentInputTimeRemaining());
    // HandleSequenceIndexChanged(RitualAltar->CurrentSequenceIndex);
}

UFUNCTION(NetMulticast, Reliable)
void Multicast_CurrentActivePlayerChanged(const ACharacter* Character, FGameplayTag ExpectedInput); 