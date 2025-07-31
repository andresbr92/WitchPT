// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "WitchPT_MVVMViewModelBase.h"
#include "UI/WidgetControllers/RitualWidgetController.h"
#include "MVVM_RitualViewModel.generated.h"

class ARitualAltar;
/**
 * 
 */
UCLASS()
class WITCHPT_API UMVVM_RitualViewModel : public UWitchPT_MVVMViewModelBase
{
	GENERATED_BODY()
public:
	virtual void UnbindCallbacksFromDependencies() override;
	UFUNCTION()
	void HandleSequenceProgressChanged(float ProgressPercentage);
	UFUNCTION()
	void HandleCorruptionChanged(float CorruptionPercentage);

	virtual void BindCallbacksToDependencies() override;
	virtual void BroadcastInitialValues() override;
private:
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess))
	EInteractionState CurrentRitualState = EInteractionState::Inactive;
	
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess))
	FString ExpectedInput;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess))
	float ProgressPercentage = 0.0f;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess))
	float CorruptionPercentage = 0.0f;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess))
	float CurrentInputTimeRemaining = 0.0f;

	UPROPERTY()
	TObjectPtr<ARitualAltar> RitualAltar = nullptr;

	
	

public:
	// -------------------- Getters and Setters ------------------------
	void SetCurrentRitualState(EInteractionState NewState);
	EInteractionState GetCurrentRitualState() const { return CurrentRitualState; }
	
	void SetExpectedInput(const FString& NewExpectedInput);
	FString GetExpectedInput() const { return ExpectedInput; }

	void SetProgressPercentage(float NewProgressPercentage) { ProgressPercentage = NewProgressPercentage; }
	float GetProgressPercentage() const { return ProgressPercentage; }

	void SetCorruptionPercentage(float NewCorruptionPercentage) { CorruptionPercentage = NewCorruptionPercentage; }
	float GetCorruptionPercentage() const { return CorruptionPercentage; }

	void SetCurrentInputTimeRemaining(float NewTimeRemaining) { CurrentInputTimeRemaining = NewTimeRemaining; }
	float GetCurrentInputTimeRemaining() const { return CurrentInputTimeRemaining; }
	
	UFUNCTION(BlueprintCallable)
	void SetRitualAltar(ARitualAltar* InRitualAltar);
	
	// -------------------- Callbacks ----------------------------------
	UFUNCTION()
	void HandleRitualStateChanged(EInteractionState NewState);
	UFUNCTION()
	void HandleTurnDataChanged(FUIRitualData TurnData);
	UFUNCTION()
	void HandleReadyPlayersChanged(FRitualReadyPlayersData ReadyPlayersData);
	UFUNCTION()
	void HandleCountdownTick(int32 CountdownValue);
	




	
	UFUNCTION()
	void HandleRitualCompleted(bool bWasSuccessful);

	// -------------------- MVVM -> Model Functions --------------------
};
