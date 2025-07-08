// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WitchPTQuickBarComponent.generated.h"


class UWitchPTInventoryItemInstance;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActiveSlotChangedSignature, int32, NewActiveSlotIndex);
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class WITCHPT_API UWitchPTQuickBarComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UWitchPTQuickBarComponent();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable, BlueprintCallable, Category="QuickBar")
	void SetActiveSlotIndex(int32 NewIndex);

	virtual void BeginPlay() override;

	UFUNCTION()
	void AddItemToSlot(int32 SlotIndex, UWitchPTInventoryItemInstance* Item);

	// --------------------- Quick Bar Delegates ---------------------
	FOnActiveSlotChangedSignature OnActiveSlotChanged;

private:
	void EquipItemInSlot();
	void UnequipItemInSlot();

protected:
	UPROPERTY()
	int32 NumSlots = 4;

	UFUNCTION()
	void OnRep_Slots();

	UFUNCTION()
	void OnRep_ActiveSlotIndex();

private:
	UPROPERTY(ReplicatedUsing=OnRep_Slots, VisibleAnywhere, Category="QuickBar")
	TArray<TObjectPtr<UWitchPTInventoryItemInstance>> Slots;
	
	UPROPERTY(ReplicatedUsing=OnRep_ActiveSlotIndex, VisibleAnywhere, Category="QuickBar")
	int32 ActiveSlotIndex = -1; // -1 means no active slot

	UPROPERTY(VisibleAnywhere, Category="QuickBar")
	TObjectPtr<UWitchPTInventoryItemInstance> EquippedItem;
	
};
