// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"

#include "WitchPT_MVVMViewModelBase.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FViewModelParams
{
	GENERATED_BODY()
	FViewModelParams() {};
	FViewModelParams(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
	:
	PlayerController(PC),
	PlayerState(PS),
	AbilitySystemComponent(ASC),
	AttributeSet(AS)
	{}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
	
};
UCLASS()
class WITCHPT_API UWitchPT_MVVMViewModelBase : public UMVVMViewModelBase
{
	GENERATED_BODY()
public:
	void SetViewModelParams(const FViewModelParams& Params);
	virtual void BindCallbacksToDependencies();
	virtual void BroadcastInitialValues();
	virtual void UnbindCallbacksFromDependencies();


protected:
	UPROPERTY(BlueprintReadOnly, Category= "ViewModel")
	TObjectPtr<APlayerController> PlayerController;
	
	UPROPERTY(BlueprintReadOnly, Category= "ViewModel")
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly, Category= "ViewModel")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category= "ViewModel")
	TObjectPtr<UAttributeSet> AttributeSet;
};
