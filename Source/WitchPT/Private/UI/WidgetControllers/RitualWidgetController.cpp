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
    // Enhanced null safety - only broadcast if we have a valid altar
    if (!IsValid(RitualAltar))
    {
        UE_LOG(LogTemp, Warning, TEXT("[RitualWidgetController] BroadcastInitialValues called with invalid altar"));
        return;
    }
    
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
    
    UE_LOG(LogTemp, Log, TEXT("[RitualWidgetController] Initial values broadcasted"));
}

void URitualWidgetController::BindCallbacksToDependencies()
{
    // Enhanced null safety - only bind if we have a valid altar
    if (!IsValid(RitualAltar))
    {
        UE_LOG(LogTemp, Warning, TEXT("[RitualWidgetController] BindCallbacksToDependencies called with invalid altar"));
        return;
    }
    
    // Bind to all the dynamic multicast delegates
    RitualAltar->OnRitualStateChangedEvent.AddDynamic(this, &URitualWidgetController::HandleRitualStateChanged);
    RitualAltar->OnReadyPlayersChangedEvent.AddDynamic(this, &URitualWidgetController::HandleReadyPlayersChanged);
    RitualAltar->OnCountdownTickEvent.AddDynamic(this, &URitualWidgetController::HandleCountdownTick);
    RitualAltar->OnTurnDataChangedEvent.AddDynamic(this, &URitualWidgetController::HandleTurnDataChanged);
    RitualAltar->OnCorruptionChangedEvent.AddDynamic(this, &URitualWidgetController::HandleCorruptionChanged);
    RitualAltar->OnSequenceProgressChangedEvent.AddDynamic(this, &URitualWidgetController::HandleSequenceProgressChanged);
    RitualAltar->OnRitualCompletedEvent.AddDynamic(this, &URitualWidgetController::HandleRitualCompleted);
    
    UE_LOG(LogTemp, Log, TEXT("[RitualWidgetController] Callbacks bound to dependencies"));
}

void URitualWidgetController::SetContextualObject(UObject* ContextualObject)
{
    if (ContextualObject)
    {
        if (ARitualAltar* InRitualAltar = Cast<ARitualAltar>(ContextualObject))
        {
            SetRitualAltar(InRitualAltar);
        }
    }
}

void URitualWidgetController::SetRitualAltar(ARitualAltar* InRitualAltar)
{
    // Enhanced null safety and cleanup
    
    // Unbind any existing callbacks first
    if (IsValid(RitualAltar))
    {
        RitualAltar->OnRitualStateChangedEvent.RemoveAll(this);
        RitualAltar->OnReadyPlayersChangedEvent.RemoveAll(this);
        RitualAltar->OnCountdownTickEvent.RemoveAll(this);
        RitualAltar->OnTurnDataChangedEvent.RemoveAll(this);
        RitualAltar->OnCorruptionChangedEvent.RemoveAll(this);
        RitualAltar->OnSequenceProgressChangedEvent.RemoveAll(this);
        RitualAltar->OnRitualCompletedEvent.RemoveAll(this);
        
        UE_LOG(LogTemp, Log, TEXT("[RitualWidgetController] Unbound callbacks from previous altar"));
    }
    
    // Assign the new altar
    RitualAltar = InRitualAltar;
    
}

bool URitualWidgetController::IsLocalPlayerActive() const
{
    // Enhanced null safety checks
    if (!IsValid(RitualAltar) || !IsValid(PlayerController))
    {
        return false;
    }
    
    // Get the local character
    ACharacter* LocalCharacter = Cast<ACharacter>(PlayerController->GetPawn());
    if (!IsValid(LocalCharacter))
    {
        return false;
    }
    
    // Check if this character is the active player
    ACharacter* ActivePlayer = RitualAltar->GetCurrentActivePlayer();
    return IsValid(ActivePlayer) && ActivePlayer == LocalCharacter;
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
    
    // Enhanced cleanup: Unbind all delegates when ritual is completed to prevent memory leaks
    if (IsValid(RitualAltar))
    {
        RitualAltar->OnRitualStateChangedEvent.RemoveAll(this);
        RitualAltar->OnReadyPlayersChangedEvent.RemoveAll(this);
        RitualAltar->OnCountdownTickEvent.RemoveAll(this);
        RitualAltar->OnTurnDataChangedEvent.RemoveAll(this);
        RitualAltar->OnCorruptionChangedEvent.RemoveAll(this);
        RitualAltar->OnSequenceProgressChangedEvent.RemoveAll(this);
        RitualAltar->OnRitualCompletedEvent.RemoveAll(this);
        
        UE_LOG(LogTemp, Log, TEXT("[RitualWidgetController] Unbound all delegates after ritual completion"));
    }
    
    // Clear the altar reference
    RitualAltar = nullptr;
}


