// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "WitchPTWidgetController.h"
#include "Item/RitualAltar.h"
#include "RitualWidgetController.generated.h"

enum class ERitualInput : uint8;
class ARitualAltar;
class ACharacter;
enum class EInteractionState : uint8;

// Delegate to notify when the expected input in the ritual changes
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRitualExpectedInputChangedSignature_WC, FGameplayTag, ExpectedInput);
// Delegate to notify when the ready players count changes
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReadyPlayersChangedSignature_WC, FRitualReadyPlayersData, ReadyPlayersData);
// Delegate to notify when the countdown ticks
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRitualCountdownTickSignature_WC, int32, CountdownValue);
// Delegate to notify when turn data changes
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTurnDataChangedSignature_WC, FUIRitualData, TurnData);
// Delegate to notify ritual state changes
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRitualStateChangedSignature_WC, EInteractionState, NewState);
// Delegate to notify corruption changes
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRitualCorruptionChangedSignature_WC, float, CorruptionPercentage);
// Delegate to notify sequence progress changes
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRitualSequenceProgressChangedSignature_WC, float, ProgressPercentage);
// Delegate to notify when the ritual is completed
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRitualCompletedSignature_WC, bool, bWasSuccessful);

/**
 * Widget Controller to manage the ritual UI
 */
UCLASS(BlueprintType, Blueprintable)
class WITCHPT_API URitualWidgetController : public UWitchPTWidgetController
{
	GENERATED_BODY()
	
public:
	URitualWidgetController();
	
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
	
	// Method to set the associated ritual altar
	UFUNCTION(BlueprintCallable, Category = "Ritual")
	void SetRitualAltar(ARitualAltar* InRitualAltar);
	
	// Getter for the current ritual altar
	UFUNCTION(BlueprintPure, Category = "Ritual")
	ARitualAltar* GetRitualAltar() const { return RitualAltar; }
	
	// Checks if the current player is the active player in the ritual
	UFUNCTION(BlueprintPure, Category = "Ritual")
	bool IsLocalPlayerActive() const;
	
	// Delegates for widgets to subscribe to
	UPROPERTY(BlueprintAssignable, Category = "Ritual")
	FOnRitualExpectedInputChangedSignature_WC OnRitualExpectedInputChanged;

	UPROPERTY(BlueprintAssignable, Category = "Ritual")
	FOnReadyPlayersChangedSignature_WC OnReadyPlayersChanged;

	UPROPERTY(BlueprintAssignable, Category = "Ritual")
	FOnRitualCountdownTickSignature_WC OnRitualCountdownTick;
	
	UPROPERTY(BlueprintAssignable, Category = "Ritual")
	FOnTurnDataChangedSignature_WC OnTurnDataChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "Ritual")
	FOnRitualStateChangedSignature_WC OnRitualStateChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "Ritual")
	FOnRitualCorruptionChangedSignature_WC OnRitualCorruptionChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "Ritual")
	FOnRitualSequenceProgressChangedSignature_WC OnRitualSequenceProgressChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "Ritual")
	FOnRitualCompletedSignature_WC OnRitualCompleted;

protected:
	// Reference to the ritual altar that this widget controls
	UPROPERTY(BlueprintReadOnly, Category = "Ritual")
	TObjectPtr<ARitualAltar> RitualAltar;
	
	// Callbacks for altar changes
	UFUNCTION()
	void HandleRitualStateChanged(EInteractionState NewState);
	
	UFUNCTION()
	void HandleReadyPlayersChanged(FRitualReadyPlayersData ReadyPlayersData);
	
	UFUNCTION()
	void HandleCountdownTick(int32 CountdownValue);
	
	UFUNCTION()
	void HandleTurnDataChanged(FUIRitualData TurnData);
	
	UFUNCTION()
	void HandleCorruptionChanged(float CorruptionPercentage);
	
	UFUNCTION()
	void HandleSequenceProgressChanged(float ProgressPercentage);
	
	UFUNCTION()
	void HandleRitualCompleted(bool bWasSuccessful);
	
private:
	// Helper function to determine if local player's turn and update turn data accordingly
	FUIRitualData ProcessTurnDataForLocalPlayer(const FUIRitualData& InTurnData) const;
}; 