// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WitchPT_MVVMViewModelBase.h"
#include "Blueprint/UserWidget.h"
#include "MVVM_CauldronViewModel.generated.h"
class UWitchPTInventoryItemInstance;
class ACauldronAltar;
/**
 * 
 */
UCLASS()
class WITCHPT_API UMVVM_CauldronViewModel : public UWitchPT_MVVMViewModelBase
{
	GENERATED_BODY()
public:
	virtual void BindCallbacksToDependencies() override;
	virtual void BroadcastInitialValues() override;
	virtual void UnbindCallbacksFromDependencies() override;

private:
	
	UPROPERTY()
	TObjectPtr<ACauldronAltar> CauldronAltar;
	
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess))
	UWitchPTInventoryItemInstance* BaseIngredient;
	
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess))
	TSubclassOf<UUserWidget> BaseIngredientIcon;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess))
	TSubclassOf<UUserWidget> PrincipalIngredientIcon;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess))
	TSubclassOf<UUserWidget> ModifierIngredientIcon;

public:
	// -------------------- Getters and Setters ------------------------
	
	void SetCauldronAltar(ACauldronAltar* InCauldronAltar);

	void SetBaseIngredient(UWitchPTInventoryItemInstance* InBaseIngredient);
	UWitchPTInventoryItemInstance* GetBaseIngredient() const { return BaseIngredient; }
	
	void SetBaseIngredientIcon(TSubclassOf<UUserWidget> InBaseIngredientIcon);
	TSubclassOf<UUserWidget> GetBaseIngredientIcon() const { return BaseIngredientIcon; }
	
	
	void SetPrincipalIngredientIcon(TSubclassOf<UUserWidget> InPrincipalIngredientIcon);
	TSubclassOf<UUserWidget> GetPrincipalIngredientIcon() const { return PrincipalIngredientIcon; }
	
	
	void SetModifierIngredientIcon(TSubclassOf<UUserWidget> InModifierIngredientIcon);
	TSubclassOf<UUserWidget> GetModifierIngredientIcon() const { return ModifierIngredientIcon; }



	
	// -------------------- Callbacks ----------------------------------
	// -------------------- MVVM -> Model Functions --------------------
};
