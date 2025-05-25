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
        OnRitualStateChanged.Broadcast(RitualAltar->GetCurrentRitualState());
        
        // Broadcast ready players data
        FRitualReadyPlayersData ReadyPlayersData;
        ReadyPlayersData.TotalPlayers = RitualAltar->GetNumberOfTotalPlayers();
        ReadyPlayersData.ReadyPlayers = RitualAltar->GetNumberOfReadyPlayers();
        OnReadyPlayersChanged.Broadcast(ReadyPlayersData);
        
        // Broadcast current turn data (processed for local player)
        FUIRitualData CurrentTurnData = RitualAltar->GetCurrentTurnData();
        FUIRitualData ProcessedTurnData = ProcessTurnDataForLocalPlayer(CurrentTurnData);
        OnTurnDataChanged.Broadcast(ProcessedTurnData);
        
        // Broadcast corruption
        OnRitualCorruptionChanged.Broadcast(RitualAltar->GetCorruptionPercentage());
        
        // Broadcast sequence progress
        OnRitualSequenceProgressChanged.Broadcast(RitualAltar->GetCurrentSequenceProgress());
    }
}

void URitualWidgetController::BindCallbacksToDependencies()
{
    // Bind to altar delegates if it exists
    if (RitualAltar)
    {
        // Bind to all the new dynamic multicast delegates
        RitualAltar->OnRitualStateChangedEvent.AddDynamic(this, &URitualWidgetController::HandleRitualStateChanged);
        RitualAltar->OnReadyPlayersChangedEvent.AddDynamic(this, &URitualWidgetController::HandleReadyPlayersChanged);
        RitualAltar->OnCountdownTickEvent.AddDynamic(this, &URitualWidgetController::HandleCountdownTick);
        RitualAltar->OnTurnDataChangedEvent.AddDynamic(this, &URitualWidgetController::HandleTurnDataChanged);
        RitualAltar->OnCorruptionChangedEvent.AddDynamic(this, &URitualWidgetController::HandleCorruptionChanged);
        RitualAltar->OnSequenceProgressChangedEvent.AddDynamic(this, &URitualWidgetController::HandleSequenceProgressChanged);
        RitualAltar->OnRitualCompletedEvent.AddDynamic(this, &URitualWidgetController::HandleRitualCompleted);
    }
}

void URitualWidgetController::SetRitualAltar(ARitualAltar* InRitualAltar)
{
    // Unbind any existing callbacks first
    if (RitualAltar != nullptr)
    {
        RitualAltar->OnRitualStateChangedEvent.RemoveAll(this);
        RitualAltar->OnReadyPlayersChangedEvent.RemoveAll(this);
        RitualAltar->OnCountdownTickEvent.RemoveAll(this);
        RitualAltar->OnTurnDataChangedEvent.RemoveAll(this);
        RitualAltar->OnCorruptionChangedEvent.RemoveAll(this);
        RitualAltar->OnSequenceProgressChangedEvent.RemoveAll(this);
        RitualAltar->OnRitualCompletedEvent.RemoveAll(this);
    }
    
    // Assign the new altar
    RitualAltar = InRitualAltar;
    
    // Rebind and broadcast if we have a valid altar
    if (RitualAltar)
    {
        BindCallbacksToDependencies();
        BroadcastInitialValues();
    }
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

FUIRitualData URitualWidgetController::ProcessTurnDataForLocalPlayer(const FUIRitualData& InTurnData) const
{
    FUIRitualData ProcessedData = InTurnData;
    
    // Determine if it's the local player's turn
    ProcessedData.bIsMyTurn = IsLocalPlayerActive();
    
    // If it's not the local player's turn, clear sensitive data
    if (!ProcessedData.bIsMyTurn)
    {
        ProcessedData.ExpectedInput = FGameplayTag::EmptyTag;
        ProcessedData.CurrentInputTimeRemaining = 0.0f;
    }
    
    return ProcessedData;
}

// ----------------------------------- CALLBACK HANDLERS ---------------------------------------------- //

void URitualWidgetController::HandleRitualStateChanged(EInteractionState NewState)
{
    OnRitualStateChanged.Broadcast(NewState);
}

void URitualWidgetController::HandleReadyPlayersChanged(FRitualReadyPlayersData ReadyPlayersData)
{
    OnReadyPlayersChanged.Broadcast(ReadyPlayersData);
}

void URitualWidgetController::HandleCountdownTick(int32 CountdownValue)
{
    OnRitualCountdownTick.Broadcast(CountdownValue);
}

void URitualWidgetController::HandleTurnDataChanged(FUIRitualData TurnData)
{
    // Process the turn data for the local player before broadcasting
    FUIRitualData ProcessedTurnData = ProcessTurnDataForLocalPlayer(TurnData);
    OnTurnDataChanged.Broadcast(ProcessedTurnData);
    
    // Also broadcast the expected input separately for backward compatibility
    if (ProcessedTurnData.bIsMyTurn)
    {
        OnRitualExpectedInputChanged.Broadcast(ProcessedTurnData.ExpectedInput);
    }
    else
    {
        OnRitualExpectedInputChanged.Broadcast(FGameplayTag::EmptyTag);
    }
}

void URitualWidgetController::HandleCorruptionChanged(float CorruptionPercentage)
{
    OnRitualCorruptionChanged.Broadcast(CorruptionPercentage);
}

void URitualWidgetController::HandleSequenceProgressChanged(float ProgressPercentage)
{
    OnRitualSequenceProgressChanged.Broadcast(ProgressPercentage);
}

void URitualWidgetController::HandleRitualCompleted(bool bWasSuccessful)
{
    OnRitualCompleted.Broadcast(bWasSuccessful);
    
    // Unbind all delegates when ritual is completed to prevent memory leaks
    if (RitualAltar)
    {
        RitualAltar->OnRitualStateChangedEvent.RemoveAll(this);
        RitualAltar->OnReadyPlayersChangedEvent.RemoveAll(this);
        RitualAltar->OnCountdownTickEvent.RemoveAll(this);
        RitualAltar->OnTurnDataChangedEvent.RemoveAll(this);
        RitualAltar->OnCorruptionChangedEvent.RemoveAll(this);
        RitualAltar->OnSequenceProgressChangedEvent.RemoveAll(this);
        RitualAltar->OnRitualCompletedEvent.RemoveAll(this);
    }
}


