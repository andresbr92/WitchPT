// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Templates/SubclassOf.h"
#include "UObject/Interface.h"
#include "UObject/ObjectPtr.h"
#include "Collectable.generated.h"
template <typename InterfaceType> class TScriptInterface;
// This class does not need to be modified.

class AActor;
class UWitchPTInventoryItemDefinition;
class UWitchPTInventoryItemInstance;
class UWitchPTInventoryManagerComponent;
class UObject;
struct FFrame;
// ------------------------- STRUCTS ---------------------- //
USTRUCT(BlueprintType)
struct FItemManifest
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UWitchPTInventoryItemDefinition> ItemDef;
};


UINTERFACE(MinimalAPI, BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class UCollectable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class WITCHPT_API ICollectable
{
	GENERATED_BODY()

public:
	/**
	 * This is the functions you call from the GA_Interact_Collect
	 */
	UFUNCTION(BlueprintCallable)
	virtual FItemManifest GetPickupInventory() const = 0;
};

UCLASS()
class UCollectableStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UCollectableStatics();
	
	UFUNCTION(BlueprintPure)
	static TScriptInterface<ICollectable> GetFirstPickupableFromActor(AActor* Actor);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, meta = (WorldContext = "Ability"))
	static void AddPickupToInventory(UWitchPTInventoryManagerComponent* InventoryComponent, TScriptInterface<ICollectable> Collectable);
};
