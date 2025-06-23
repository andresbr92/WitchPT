// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/WitchPTUserWidget.h"
#include "RitualUserWidget.generated.h"

class ARitualAltar;

/**
 * User Widget for ritual interactions - enhanced for visibility management
 */
UCLASS()
class WITCHPT_API URitualUserWidget : public UWitchPTUserWidget
{
	GENERATED_BODY()

public:
	URitualUserWidget(const FObjectInitializer& ObjectInitializer);

	// Enhanced visibility management methods
	UFUNCTION(BlueprintCallable, Category = "Ritual Widget")
	void ShowForRitual(ARitualAltar* Altar);
	
	UFUNCTION(BlueprintCallable, Category = "Ritual Widget")
	void HideFromRitual();
	
	// Check if widget is currently active for a ritual
	UFUNCTION(BlueprintPure, Category = "Ritual Widget")
	bool IsActiveForRitual() const { return bIsActiveForRitual; }
	
	// Get the current ritual altar (if any)
	UFUNCTION(BlueprintPure, Category = "Ritual Widget")
	ARitualAltar* GetCurrentRitualAltar() const { return CurrentRitualAltar; }

protected:
	// Override from UWitchPTUserWidget for enhanced visibility management
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	
	// Called when the widget is shown for a ritual
	UFUNCTION(BlueprintImplementableEvent, Category = "Ritual Widget")
	void OnShownForRitual(ARitualAltar* Altar);
	
	// Called when the widget is hidden from a ritual
	UFUNCTION(BlueprintImplementableEvent, Category = "Ritual Widget")
	void OnHiddenFromRitual();
	
	// Reset widget state when switching between rituals
	UFUNCTION(BlueprintCallable, Category = "Ritual Widget")
	void ResetWidgetState();

private:
	// Track if widget is currently active for a ritual
	UPROPERTY()
	bool bIsActiveForRitual = false;
	
	// Reference to the current ritual altar
	UPROPERTY()
	TObjectPtr<ARitualAltar> CurrentRitualAltar;
};
