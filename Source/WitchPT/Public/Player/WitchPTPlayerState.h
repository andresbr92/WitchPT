// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "WitchPTPlayerState.generated.h"
class ARitualPosition;
class ARitualAltar;
class UAbilitySystemComponent;
class UAttributeSet;
/**
 * 
 */
UCLASS()
class WITCHPT_API AWitchPTPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	AWitchPTPlayerState();
public:

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	// Setters
	UFUNCTION(BlueprintCallable)
	void SetRitualAltarInteractingWith(ARitualPosition* RitualPosition);
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TObjectPtr<ARitualAltar> RitualAltarInteractingWith;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TObjectPtr<ARitualPosition> RitualPositionInteractingWith;
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAttributeSet> AttributeSet;
	
	
};
