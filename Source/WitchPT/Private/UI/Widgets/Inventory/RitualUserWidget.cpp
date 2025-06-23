// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Widgets/Inventory/RitualUserWidget.h"
#include "Item/RitualAltar.h"
#include "Components/Widget.h"

URitualUserWidget::URitualUserWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , bIsActiveForRitual(false)
    , CurrentRitualAltar(nullptr)
{
    // Constructor
}

void URitualUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    
    // Initialize widget state
    ResetWidgetState();
}

void URitualUserWidget::NativeDestruct()
{
    // Clean up when widget is destroyed
    HideFromRitual();
    
    Super::NativeDestruct();
}

void URitualUserWidget::ShowForRitual(ARitualAltar* Altar)
{
    if (!Altar)
    {
        UE_LOG(LogTemp, Warning, TEXT("[RitualUserWidget] ShowForRitual called with null altar"));
        return;
    }
    
    // If we're already active for a different ritual, reset first
    if (bIsActiveForRitual && CurrentRitualAltar != Altar)
    {
        ResetWidgetState();
    }
    
    // Set up for the new ritual
    CurrentRitualAltar = Altar;
    bIsActiveForRitual = true;
    
    // Call Blueprint implementable event
    OnShownForRitual(Altar);
    
    UE_LOG(LogTemp, Log, TEXT("[RitualUserWidget] Shown for ritual altar"));
}

void URitualUserWidget::HideFromRitual()
{
    if (!bIsActiveForRitual)
    {
        return; // Already hidden
    }
    
    // Clear state
    bIsActiveForRitual = false;
    ARitualAltar* PreviousAltar = CurrentRitualAltar;
    CurrentRitualAltar = nullptr;
    
    // Call Blueprint implementable event
    OnHiddenFromRitual();
    
    // Reset widget state
    ResetWidgetState();
    
    UE_LOG(LogTemp, Log, TEXT("[RitualUserWidget] Hidden from ritual"));
}

void URitualUserWidget::ResetWidgetState()
{
    // Reset any internal widget state here
    // This ensures the widget is clean for the next use
    
    // Clear any cached data or temporary state
    // Blueprint implementers can override this for custom reset logic
    
    UE_LOG(LogTemp, Log, TEXT("[RitualUserWidget] Widget state reset"));
}
