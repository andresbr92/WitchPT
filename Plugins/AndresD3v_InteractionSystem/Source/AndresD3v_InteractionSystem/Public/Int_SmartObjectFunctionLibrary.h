// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Int_SmartObjectFunctionLibrary.generated.h"

struct FSmartObjectSlotHandle;
struct FSmartObjectRequestResult;
class UInt_InteractionDefinition;
struct FSmartObjectHandle;
struct FSmartObjectRequestFilter;
/**
 * 
 */
UCLASS()
class ANDRESD3V_INTERACTIONSYSTEM_API UInt_SmartObjectFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/**
	 * Searches for smart object slots with interaction entrances on an actor.
	 * @param Filter The search filter.
	 * @param SearchActor The actor to search.
	 * @param OutResults The found smart object slot candidates (output).
	 * @param UserActor Optional actor for additional data in condition evaluation.
	 * @return True if at least one candidate was found, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category="AndresD3v|DmartObject", meta=(DeterminesOutputType="DesiredClass", DynamicOutputParam="OutConfig", ExpandBoolAsExecs="ReturnValue"))
	static bool FindSmartObjectsWithInteractionEntranceInActor(const FSmartObjectRequestFilter& Filter, AActor* SearchActor, TArray<FSmartObjectRequestResult>& OutResults,
															   const AActor* UserActor = nullptr);

	/**
	 * 
	 * @param WorldContext The world context object.
	 * @param SmartObjectSlotHandle The smart object slot handle.
	 * @param OutDefinition The interaction definition (output).
	 * @return 
	 */
	UFUNCTION(BlueprintCallable, Category="AndresD3v|DmartObject", meta=(WorldContext="WorldContext", ExpandBoolAsExecs="ReturnValue"))
	static bool FindInteractionDefinitionFromSmartObjectSlot(UObject* WorldContext, FSmartObjectSlotHandle SmartObjectSlotHandle, UInt_InteractionDefinition*& OutDefinition);
};
