// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WitchPTPlayerController.generated.h"

class UWitchPTAbilitySystemComponent;
class UWitchPTInputConfig;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class WITCHPT_API AWitchPTPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AWitchPTPlayerController();
	virtual void PlayerTick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	// Actions
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> WitchPtiInputMappingContext;
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, Category= "Input")
	TObjectPtr<UInputAction> LookAction;
	//InputConfig
	UPROPERTY(EditDefaultsOnly, Category= "Input")
	TObjectPtr<UWitchPTInputConfig> InputConfig;

	// Ability System Component
	UPROPERTY()
	TObjectPtr<UWitchPTAbilitySystemComponent> WitchPtAbilitySystemComponent;

	UWitchPTAbilitySystemComponent* GetASC();
	

	void Move(const FInputActionValue& InputActionValue);

	void Look(const FInputActionValue& Value);
	
};

