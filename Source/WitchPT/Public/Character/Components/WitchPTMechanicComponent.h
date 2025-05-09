// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Item/MechanicsInterface.h"
#include "WitchPTMechanicComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WITCHPT_API UWitchPTMechanicComponent : public UActorComponent, public IMechanicsInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWitchPTMechanicComponent();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	// --- RITUAL IMPLEMENTATION ---
	virtual ARitualAltar* GetRitualAltarActor_Implementation() const override;
	virtual void SendStartRitualRequest_Implementation(ACharacter* RequestingCharacter) override;
	virtual void SendPlayerInput_Implementation(ACharacter* InputCharacter, const FGameplayTag& InputTag) override;
	virtual void SendPlayerOccupiedPosition_Implementation(ACharacter* InputCharacter) override;
	virtual bool IsOccuppied_Implementation() const override;
	// --- CAULDRON IMPLEMENTATION ---
	virtual void SendStartBrewingPotionRequest_Implementation(ACharacter* RequestingCharacter) override;
	virtual void SendStartCarryCauldronRequest_Implementation(ACharacter* RequestingCharacter) override;
	virtual void SendStartPlacementPreview_Implementation(ACharacter* RequestingCharacter) override;
	virtual void SendUpdatePlacementPreview_Implementation(const FVector& HitLocation, const FVector& HitNormal) override;
	virtual void SendCancelPlacementPreview_Implementation() override;
	virtual void SendFinalizePlacement_Implementation() override;
	

protected:

	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
