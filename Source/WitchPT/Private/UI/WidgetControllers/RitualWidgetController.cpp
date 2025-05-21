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
        RitualAltar->OnReadyPlayersNumberChangedDelegate.BindLambda([this](int32 TotalPlayers, int32 PlayersReady)
        {
            OnReadyPlayersNumberChangedSignature.Broadcast(TotalPlayers, PlayersReady);
        });

        RitualAltar->OnCurrentActivePlayerChanged.BindUObject(this, &URitualWidgetController::HandleActivePlayerChanged);
        // BindLambda([this](const ACharacter* Character, FGameplayTag ExpectedInput)
        // {
        //         // Broadcast true if the player controller of the character is the same
        //         ACharacter* LocalCharacter = Cast<ACharacter>(PlayerController->GetPawn());
        //         if (LocalCharacter && LocalCharacter == Character && RitualAltar->GetCurrentRitualState() == EInteractionState::Active)
        //        
        //         {
        //             OnIsMyTurnChangedSignature.Broadcast(true, ExpectedInput);
        //             
        //         }
        //         else
        //         {
        //             OnIsMyTurnChangedSignature.Broadcast(false, FGameplayTag::EmptyTag);
        //         }
        //         
        // });
        RitualAltar->OnCurrentSequenceIndexChanged.BindLambda([this](int32 NewIndex)
        {
            HandleSequenceIndexChanged(NewIndex);
        });

        
       
        
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
    BindCallbacksToDependencies();
    // BroadcastInitialValues();
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

void URitualWidgetController::HandleActivePlayerChanged(const ACharacter* NewActivePlayer, FGameplayTag ExpectedInput) const
{
    
    // Broadcast true if the player controller of the character is the same
    ACharacter* LocalCharacter = Cast<ACharacter>(PlayerController->GetPawn());
    if (LocalCharacter && LocalCharacter == NewActivePlayer)
    
    {
        OnIsMyTurnChangedSignature.Broadcast(true, RitualAltar->GetCurrentExpectedInput());
        
    }
    else
    {
        OnIsMyTurnChangedSignature.Broadcast(false, FGameplayTag::EmptyTag);
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