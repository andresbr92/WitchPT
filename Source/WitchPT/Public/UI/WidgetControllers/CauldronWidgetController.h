// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WitchPTWidgetController.h"
#include "CauldronWidgetController.generated.h"

class ACauldronAltar;
class UWitchPTInventoryItemInstance;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBaseIngredientDropped_WC, UWitchPTInventoryItemInstance*, IngredientInstance);
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class WITCHPT_API UCauldronWidgetController : public UWitchPTWidgetController
{
	GENERATED_BODY()
public:
	UCauldronWidgetController();
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	// Delegates for widgets to subscribe to
	UPROPERTY(BlueprintAssignable, Category = "Cauldron")
	FOnBaseIngredientDropped_WC OnBaseIngredientDropped;

	UFUNCTION(Category = "Cauldron")
	void SetCauldronAltar(ACauldronAltar* InCauldronAltar);

	// Getter for the current ritual altar
	UFUNCTION(BlueprintPure, Category = "Cauldron")
	ACauldronAltar* GetCauldronAltar() const { return CauldronAltar; }


protected:
	// Reference to the ritual altar that this widget controls
	UPROPERTY(BlueprintReadOnly, Category = "Cauldron")
	TObjectPtr<ACauldronAltar> CauldronAltar;

	UFUNCTION(Category = "Cauldron")
	void HandleBaseIngredientDropped(UWitchPTInventoryItemInstance* IngredientInstance);
};
