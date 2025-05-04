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
class WITCHPT_API AWitchPTCharacter : public AWitchPTCharacterBase, public IMechanicsInterface
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



	/***
	 * IMechanicsInterface implementation
	 */
	/***
	 * Ritual Altar interface functions
	 */
	virtual void HandleStartRitualRequest_Implementation(ACharacter* RequestingCharacter) override;
	virtual void HandlePlayerInput_Implementation(ACharacter* InputCharacter, const FGameplayTag& InputTag) override;
	virtual void HandleSetRitualPosition_Implementation(ARitualPosition* InRitualPosition) override;
	/***
	 * Ritual Position interface functions
	 */
	virtual bool IsRitualPositionOccupied_Implementation() const override;
	virtual void SetRitualPositionOccupied_Implementation(ACharacter* OccupyingCharacter) override;
	virtual ARitualPosition* GetRitualPosition_Implementation() const override { return RitualPosition;  }
	/***
	 * Cauldron Altar interface functions
	 */

	virtual ACauldronAltar* GetCauldronAltar_Implementation() override { return CauldronAltar; }
	virtual void HandleSetCauldronAltar_Implementation(ACauldronAltar* InCauldronAltar) override;
	virtual void HandleRemoveCauldronAltar_Implementation() override { CauldronAltar = nullptr; }

	


	//RPC
	UFUNCTION(Server, Reliable)
	void Server_HandleStartRitualRequest(ACharacter* RequestingCharacter);

	UFUNCTION(Server, Reliable)
	void Server_HandlePlayerInput(ACharacter* InputCharacter, const FGameplayTag& InputTag);

	UFUNCTION(Server, Reliable)
	void Server_SetRitualPositionOccupied(ACharacter* Player, ARitualPosition* Position);

	

	

	


};
