// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WitchPTWidgetController.h"
#include "CauldronWidgetController.generated.h"

class ACauldronAltar;
class UWitchPTInventoryItemInstance;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBaseIngredientSetSignature_WC, UWitchPTInventoryItemInstance*, IngredientInstance);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBaseIngredientIconSetSignature_WC, TSubclassOf<UUserWidget>, BaseIngredientIcon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPrincipalIngredientSetSignature_WC, UWitchPTInventoryItemInstance*, IngredientInstance);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPrincipalIngredientIconSetSignature_WC, TSubclassOf<UUserWidget>, PrincipalIngredientIcon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnModifierIngredientSetSignature_WC, UWitchPTInventoryItemInstance*, IngredientInstance);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnModifierIngredientIconSetSignature_WC, TSubclassOf<UUserWidget>, ModifierIngredientIcon);
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
	void UnbindCallbacksFromDependencies();

	// Delegates for widgets to subscribe to
	UPROPERTY(BlueprintAssignable, Category = "Cauldron")
	FOnBaseIngredientSetSignature_WC OnBaseIngredientSet;
	UPROPERTY(BlueprintAssignable, Category = "Cauldron")
	FOnBaseIngredientIconSetSignature_WC OnBaseIngredientIconSet;
	UPROPERTY(BlueprintAssignable, Category = "Cauldron")
	FOnPrincipalIngredientSetSignature_WC OnPrincipalIngredientSet;
	UPROPERTY(BlueprintAssignable, Category = "Cauldron")
	FOnPrincipalIngredientIconSetSignature_WC OnPrincipalIngredientIconSet;
	UPROPERTY(BlueprintAssignable, Category = "Cauldron")
	FOnModifierIngredientSetSignature_WC OnModifierIngredientSet;
	UPROPERTY(BlueprintAssignable, Category = "Cauldron")
	FOnModifierIngredientIconSetSignature_WC OnModifierIngredientIconSet;

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
	void HandleBaseIngredientSet(UWitchPTInventoryItemInstance* IngredientInstance);
	UFUNCTION(Category = "Cauldron")
	void HandleBaseIngredientIconSet(TSubclassOf<UUserWidget> BaseIngredientIcon);
	UFUNCTION(Category = "Cauldron")
	void HandlePrincipalIngredientSet(UWitchPTInventoryItemInstance* IngredientInstance);
	UFUNCTION(Category = "Cauldron")
	void HandlePrincipalIngredientIconSet(TSubclassOf<UUserWidget> PrincipalIngredientIcon);
	UFUNCTION(Category = "Cauldron")
	void HandleModifierIngredientSet(UWitchPTInventoryItemInstance* IngredientInstance);
	UFUNCTION(Category = "Cauldron")
	void HandleModifierIngredientIconSet(TSubclassOf<UUserWidget> ModifierIngredientIcon);
};
