// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "WitchPTWidgetController.h"
#include "RitualWidgetController.generated.h"

enum class ERitualInput : uint8;
class ARitualAltar;
class ACharacter;
enum class EInteractionState : uint8;

// Delegate to notify when the expected input in the ritual changes
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRitualExpectedInputChangedSignature_WC, FGameplayTag, ExpectedInput);
// Delegate to notify when the expected input in the ritual changes
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnNumberOfReadyPlayersNumberChangedSignature_WC, int32, TotalPlayers, int32, PlayersReady);
// Delegate to notify when the countDown start and with the value
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRitualCountdownTickSignature_WC, int32, CountdownValue);

// Delegate to notify when the active player changes
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnIsMyTurnChangedSignature_WC, bool, bIsMyTurnChanged, FGameplayTag, ExpectedInput, float, RitualPercentageCompleted);

// Delegate to notify ritual state changes
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRitualStateChangedSignature_WC, EInteractionState, NewState);

// Delegate to notify input timer changes
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRitualInputTimerChangedSignature_WC, float, RemainingTime);

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
	UFUNCTION(Category = "Ritual")
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
	FOnNumberOfReadyPlayersNumberChangedSignature_WC OnReadyPlayersNumberChangedSignature;

	UPROPERTY(BlueprintAssignable, Category = "Ritual")
	FOnRitualCountdownTickSignature_WC OnRitualCountdownTickDelegate;
	
	UPROPERTY(BlueprintAssignable, Category = "Ritual")
	FOnIsMyTurnChangedSignature_WC OnIsMyTurnChangedDelegate;
	
	UPROPERTY(BlueprintAssignable, Category = "Ritual")
	FOnRitualStateChangedSignature_WC OnRitualStateChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "Ritual")
	FOnRitualInputTimerChangedSignature_WC OnRitualInputTimerChanged;
	
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
	
	
	void HandleActivePlayerChanged(const bool IsMyTurnChanged, const FGameplayTag ExpectedInput, float RitualPercentageCompleted) const;
	
	UFUNCTION()
	void HandleInputTimerChanged(float NewTime);
	
	UFUNCTION()
	void HandleCorruptionChanged(float NewCorruption);
	
	UFUNCTION()
	void HandleSequenceIndexChanged(int32 NewIndex);
	
	UFUNCTION()
	void HandleRitualCompleted(bool bWasSuccessful);

	UFUNCTION()
	void HandleNumberOfReadyPlayersChanged(int32 TotalPlayers, int32 PlayersReady);
	
	// Callback for AbilitySystem events
	void HandleTurnAdvanced(FGameplayTag Tag, int32 NewCount);
}; 