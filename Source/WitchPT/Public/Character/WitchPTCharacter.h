// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WitchPTCharacterBase.h"
#include "Item/CauldronAltar.h"
#include "Item/MechanicsInterface.h"
#include "WitchPTCharacter.generated.h"

class ACauldronAltar;
class ARitualPosition;

UCLASS()
class WITCHPT_API AWitchPTCharacter : public AWitchPTCharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWitchPTCharacter(const class FObjectInitializer& ObjectInitializer);
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	
	virtual void InitAbilityActorInfo() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ritual")
	TObjectPtr<ARitualPosition> RitualPosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cauldron")
	TObjectPtr<ACauldronAltar> CauldronAltar;





	


};
