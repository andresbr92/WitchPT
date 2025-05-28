// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Item/MechanicsInterface.h"
#include "WitchPTMechanicComponent.generated.h"

class ACauldronAltar;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WITCHPT_API UWitchPTMechanicComponent : public UActorComponent, public IMechanicsInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWitchPTMechanicComponent();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	// ----------------------------------- CAULDRON IMPLEMENTATION ---------------------------------
	virtual void RequestStartBrewingPotion_Implementation(ACauldronAltar* TargetAltar) override;
	virtual void RequestStartCarryCauldron_Implementation(ACauldronAltar* TargetAltar) override;
	virtual void RequestStartPlacementPreview_Implementation(ACauldronAltar* TargetAltar) override;
	virtual void RequestUpdatePlacementPreview_Implementation(ACauldronAltar* TargetAltar, const FVector& HitLocation, const FVector& HitNormal) override;
	virtual void RequestCancelPlacementPreview_Implementation(ACauldronAltar* TargetAltar) override;
	virtual void RequestFinalizePlacement_Implementation(ACauldronAltar* TargetAltar) override;
	virtual void RequestUnOccupyPositionInCauldron_Implementation(ACauldronAltar* TargetCauldron) override;
	// ----------------------------------- CAULDRON INGREDIENT IMPLEMENTATION ---------------------------------
	virtual void RequestSetBaseIngredient_Implementation(ACauldronAltar* TargetAltar, TSubclassOf<UWitchPTInventoryItemDefinition> IngredientItemDef) override;
	
	

	
	// ------------------------------------ RITUAL IMPLEMENTATION ---------------------------------
	virtual void RequestStartRitual_Implementation(ARitualAltar* TargetAltar) override;
	virtual void RequestRitualInput_Implementation(ARitualAltar* TargetAltar, const FGameplayTag& InputTag) override;
	virtual void RequestOccupyPosition_Implementation(ARitualAltar* TargetAltar, ABaseInteractionPosition* Position) override;
	virtual void RequestUnOccupyPositionInRitual_Implementation(ARitualAltar* TargetAltar, ABaseInteractionPosition* Position) override;
	

	

protected:

	
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
