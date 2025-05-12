// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "WitchPTWidgetController.h"
#include "OverlayWidgetController.generated.h"
class UWitchPTUserWidget;

USTRUCT()
struct FUIWidgetRow: public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MessageGameplayTag = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Message = FText();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UWitchPTUserWidget> MessageWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Image = nullptr;
	
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewHealth);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMessageWidgetRowSignature, FUIWidgetRow, Row);

UCLASS(BlueprintType, Blueprintable)
class WITCHPT_API UOverlayWidgetController : public UWitchPTWidgetController
{
	GENERATED_BODY()
public:
	/**
	 * Handles the reception of asset gameplay tags from a Gameplay Effect (GE).
	 *
	 * This method is triggered when a Gameplay Effect sends a set of gameplay tags
	 * as part of its asset definition. It usually processes or reacts to the tags
	 * received within the container.
	 *
	 * @param TagContainer A container of gameplay tags associated with the triggered Gameplay Effect.
	 */
	void OnAssetTagReceivedFromGE(const FGameplayTagContainer& TagContainer) const;
	virtual void BroadcastInitialValues() override;

	virtual void BindCallbacksToDependencies() override;



	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnHealthChanged;
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnStaminaChanged;
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnMaxStaminaChanged;
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnAgeChanged;
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnMaxAgeChanged;

	UPROPERTY(BlueprintAssignable, Category="Widget Data")
	FOnMessageWidgetRowSignature OnMessageWidgetRow;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Widget Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;

	template<typename T>
T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& GameplayTag);
};

template <typename T>
T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& GameplayTag)
{
	return  DataTable->FindRow<T>(GameplayTag.GetTagName(), TEXT(""));

}























