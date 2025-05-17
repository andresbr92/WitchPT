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
        
        // Broadcast active player
        OnRitualActivePlayerChanged.Broadcast(RitualAltar->GetCurrentActivePlayer());
        
        // Broadcast expected input
        OnRitualExpectedInputChanged.Broadcast(RitualAltar->GetCurrentExpectedInput());
        
        // Broadcast remaining time
        OnRitualInputTimerChanged.Broadcast(RitualAltar->GetCurrentInputTimeRemaining());
        
        // Broadcast corruption
        OnRitualCorruptionChanged.Broadcast(RitualAltar->GetCorruptionPercentage());
        
        // Broadcast sequence progress
        OnRitualSequenceProgressChanged.Broadcast(RitualAltar->GetCurrentSequenceProgress());
    }
}

void URitualWidgetController::BindCallbacksToDependencies()
{
    // Bind to altar delegations if it exists
    if (RitualAltar)
    {
        // Subscribe to RitualAltar delegates
        
        // For ritual state changes, we can subscribe to OnRep events
        // through EventTags or create custom variables that call Multicast RPCs from the altar
        
        // Subscribe to OnRitualCompleted
        RitualAltar->OnRitualCompleted.AddDynamic(this, &URitualWidgetController::HandleRitualCompleted);
        
        // We can also watch for changes using AbilitySystem for ritual events
        if (AbilitySystemComponent)
        {
            const FWitchPTGameplayTags& GameplayTags = FWitchPTGameplayTags::Get();
            
            // Listen for turn change events
            // AbilitySystemComponent->RegisterGameplayTagEvent(
            //     GameplayTags.Event_Ritual_TurnAdvanced, 
            //     EGameplayTagEventType::NewOrRemoved
            // ).AddUObject(this, &URitualWidgetController::HandleTurnAdvanced);
        }
    }
}

void URitualWidgetController::SetRitualAltar(ARitualAltar* InRitualAltar)
{
    // Unbind any existing callbacks first
    if (RitualAltar)
    {
        RitualAltar->OnRitualCompleted.RemoveAll(this);
    }
    
    // Assign the new altar
    RitualAltar = InRitualAltar;
    
    // Rebind and broadcast
    BindCallbacksToDependencies();
    BroadcastInitialValues();
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

void URitualWidgetController::HandleActivePlayerChanged(ACharacter* NewActivePlayer)
{
    OnRitualActivePlayerChanged.Broadcast(NewActivePlayer);
    
    // If it's the local player's turn, we can also broadcast the expected input
    if (IsLocalPlayerActive() && RitualAltar)
    {
        // Get and broadcast the expected input
        FGameplayTag ExpectedInput = RitualAltar->GetCurrentExpectedInput();
        OnRitualExpectedInputChanged.Broadcast(ExpectedInput);
    }
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

void URitualWidgetController::HandleTurnAdvanced(const FGameplayTag& Tag, int32 NewCount)
{
    // Only process if the tag was added
    if (NewCount <= 0 || !RitualAltar)
    {
        return;
    }
    
    // Update UI with current values
    HandleActivePlayerChanged(RitualAltar->GetCurrentActivePlayer());
    HandleInputTimerChanged(RitualAltar->GetCurrentInputTimeRemaining());
    HandleSequenceIndexChanged(RitualAltar->CurrentSequenceIndex);
} 