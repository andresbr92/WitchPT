// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_InventoryList.generated.h"

class UInv_InventoryWidgetController;
class UMVVMViewModelBase;
class UGridPanel;
class UCanvasPanel;
/**
 * 
 */
UCLASS()
class ANDRESD3V_INVENTORYSYSTEM_API UInv_InventoryList : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	void SetViewModel(UMVVMViewModelBase* InViewModel);
	UMVVMViewModelBase* GetViewModel() const { return ViewModel; }
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TObjectPtr<UMVVMViewModelBase> ViewModel;
private:
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UGridPanel> GridPanel;

	UPROPERTY()
	TArray<TObjectPtr<UUserWidget>> GridSlots;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<UUserWidget> ItemWidgetClass; 

	UFUNCTION()
	void OnInventoryListChanged();

	UPROPERTY(BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInv_InventoryWidgetController> WidgetController;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	int32 Columns = 8;
};
