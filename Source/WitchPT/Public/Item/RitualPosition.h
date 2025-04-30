// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Item.h"
#include "AbilitySystem/Interaction/InteractionOption.h"
#include "RitualPosition.generated.h"

class ARitualAltar;
class UAbilitySystemComponent;
struct FGameplayAbilitySpec;
class AController;
class UGameplayAbility;

UCLASS()
class WITCHPT_API ARitualPosition : public AItem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARitualPosition();
	virtual void GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& OptionBuilder) override;
	
	


protected:
	UPROPERTY()
	TWeakObjectPtr<UAbilitySystemComponent> InteractingASC;

	UPROPERTY()
	TWeakObjectPtr<AController> InteractingController;

	

};
