// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryManagement/FastArray/Inv_FastArray.h"
#include "Inv_InventoryComponent.generated.h"

class UInv_InventoryWidgetController;
class UInv_ItemComponent;
class UInv_InventoryItem;
class UInv_InventoryBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryItemChange, UInv_InventoryItem*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNoRoomInInventory);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStackChange, const FInv_SlotAvailabilityResult&, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemEquipStatusChanged, UInv_InventoryItem*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryMenuToggled, bool, bOpen);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class ANDRESD3V_INVENTORYSYSTEM_API UInv_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInv_InventoryComponent();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory")
	void TryAddItem(UInv_ItemComponent* ItemComponent);

	UFUNCTION(Server, Reliable)
	void Server_AddNewItem(UInv_ItemComponent* ItemComponent, int32 StackCount, int32 Remainder);

	UFUNCTION(Server, Reliable)
	void Server_AddStacksToItem(UInv_ItemComponent* ItemComponent, int32 StackCount, int32 Remainder);

	UFUNCTION(Server, Reliable)
	void Server_DropItem(UInv_InventoryItem* Item, int32 StackCount);

	UFUNCTION(Server, Reliable)
	void Server_ConsumeItem(UInv_InventoryItem* Item);

	UFUNCTION(Server, Reliable)
	void Server_EquipSlotClicked(UInv_InventoryItem* ItemToEquip, UInv_InventoryItem* ItemToUnequip);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_EquipSlotClicked(UInv_InventoryItem* ItemToEquip, UInv_InventoryItem* ItemToUnequip);

	TArray<UInv_InventoryItem*> GetAllItems() const { return InventoryList.GetAllItems(); }

	UPROPERTY()
	TObjectPtr<UInv_InventoryWidgetController> WidgetController;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInv_InventoryWidgetController> WidgetControllerClass;


	void AddRepSubObj(UObject* SubObj);
	void SpawnDroppedItem(UInv_InventoryItem* Item, int32 StackCount);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	UInv_InventoryBase* GetInventoryMenu() const { return InventoryMenu; }

	FInventoryItemChange OnItemAdded;
	FInventoryItemChange OnItemRemoved;
	UPROPERTY(BlueprintAssignable)
	FNoRoomInInventory NoRoomInInventory;
	FStackChange OnStackChange;
	FItemEquipStatusChanged OnItemEquipped;
	FItemEquipStatusChanged OnItemUnequipped;
	FInventoryMenuToggled OnInventoryMenuToggled;
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<UInv_InventoryBase> InventoryMenuClass;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TObjectPtr<UInv_InventoryBase> InventoryMenu;

	UFUNCTION(BlueprintCallable)
	void SetInventoryMenu(UInv_InventoryBase* NewInventoryMenu);
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetInventoryOpen(bool bOpen) 
	{ 
		if (bIsInventoryOpen != bOpen)
		{
			bIsInventoryOpen = bOpen; 
			OnInventoryMenuToggled.Broadcast(bIsInventoryOpen);
		}
	}
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool IsInventoryOpen() const { return bIsInventoryOpen; }
	
	
private:
	TWeakObjectPtr<APlayerController> OwningController;

	void ConstructInventory();
	void InitializeInventoryWidgetController();
	
	UPROPERTY(Replicated)
	FInv_InventoryFastArray InventoryList;
	
	bool bIsInventoryOpen;
	

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float DropSpawnAngleMin = -85.f;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float DropSpawnAngleMax = 85.f;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float DropSpawnDistanceMin = 10.f;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float DropSpawnDistanceMax = 50.f;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float RelativeSpawnElevation = 70.f;
};
