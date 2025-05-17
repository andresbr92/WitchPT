// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "WitchPTWidgetController.h"
#include "RitualWidgetController.generated.h"

class ARitualAltar;
class ACharacter;
enum class EInteractionState : uint8;

// Delegate to notify when the expected input in the ritual changes
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRitualExpectedInputChangedSignature, FGameplayTag, ExpectedInput);

// Delegate to notify when the active player changes
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRitualActivePlayerChangedSignature, ACharacter*, ActivePlayer);

// Delegate to notify ritual state changes
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRitualStateChangedSignature, EInteractionState, NewState);

// Delegate to notify input timer changes
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRitualInputTimerChangedSignature, float, RemainingTime);

// Delegate to notify corruption changes
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRitualCorruptionChangedSignature, float, CorruptionPercentage);

// Delegate to notify sequence progress changes
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRitualSequenceProgressChangedSignature, int32, ProgressPercentage);

// Delegate to notify when the ritual is completed
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRitualCompletedSignature, bool, bWasSuccessful);

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
	FOnRitualExpectedInputChangedSignature OnRitualExpectedInputChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "Ritual")
	FOnRitualActivePlayerChangedSignature OnRitualActivePlayerChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "Ritual")
	FOnRitualStateChangedSignature OnRitualStateChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "Ritual")
	FOnRitualInputTimerChangedSignature OnRitualInputTimerChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "Ritual")
	FOnRitualCorruptionChangedSignature OnRitualCorruptionChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "Ritual")
	FOnRitualSequenceProgressChangedSignature OnRitualSequenceProgressChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "Ritual")
	FOnRitualCompletedSignature OnRitualCompleted;
	
protected:
	// Reference to the ritual altar that this widget controls
	UPROPERTY(BlueprintReadOnly, Category = "Ritual")
	TObjectPtr<ARitualAltar> RitualAltar;
	
	// Callbacks for altar changes
	UFUNCTION()
	void HandleRitualStateChanged(EInteractionState NewState);
	
	UFUNCTION()
	void HandleActivePlayerChanged(ACharacter* NewActivePlayer);
	
	UFUNCTION()
	void HandleInputTimerChanged(float NewTime);
	
	UFUNCTION()
	void HandleCorruptionChanged(float NewCorruption);
	
	UFUNCTION()
	void HandleSequenceIndexChanged(int32 NewIndex);
	
	UFUNCTION()
	void HandleRitualCompleted(bool bWasSuccessful);
	
	// Callback for AbilitySystem events
	void HandleTurnAdvanced(const FGameplayTag& Tag, int32 NewCount);
}; 