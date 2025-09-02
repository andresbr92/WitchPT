// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WitchPT_MVVMViewModelBase.h"
#include "Blueprint/UserWidget.h"
#include "MVVM_CauldronViewModel.generated.h"
class UWitchPTInventoryItemDefinition;
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
	UWitchPTInventoryItemInstance* PrincipalIngredient;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess))
	UWitchPTInventoryItemInstance* ModifierIngredient;




public:
	// -------------------- Getters and Setters ------------------------
	
	void SetCauldronAltar(ACauldronAltar* InCauldronAltar);

	void SetBaseIngredient(UWitchPTInventoryItemInstance* InBaseIngredient);
	UWitchPTInventoryItemInstance* GetBaseIngredient() const { return BaseIngredient; }

	void SetPrincipalIngredient(UWitchPTInventoryItemInstance* InPrincipalIngredient);
	UWitchPTInventoryItemInstance* GetPrincipalIngredient() const { return PrincipalIngredient; }

	void SetModifierIngredient(UWitchPTInventoryItemInstance* InModifierIngredient);
	UWitchPTInventoryItemInstance* GetModifierIngredient() const { return ModifierIngredient; }
	

	
	




	
	// -------------------- Callbacks ----------------------------------
	// -------------------- MVVM -> Model Functions --------------------
	UFUNCTION(BlueprintCallable, Category = "Cauldron|ViewModel", CallInEditor, meta = (DisplayName = "Try Set Ingredient In Slot"))
	void TrySetIngredientInSlot(const ACharacter* RequestingCharacter, const TSubclassOf<UWitchPTInventoryItemDefinition>& IngredientItemDef);
	
};
