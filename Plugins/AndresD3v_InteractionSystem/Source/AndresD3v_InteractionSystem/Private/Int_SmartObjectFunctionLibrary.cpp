// Fill out your copyright notice in the Description page of Project Settings.


#include "Int_SmartObjectFunctionLibrary.h"

#include "Int_InteractionDefinition.h"
#include "Int_InteractionStructLibrary.h"
#include "SmartObjectBlueprintFunctionLibrary.h"
#include "SmartObjectSubsystem.h"

bool UInt_SmartObjectFunctionLibrary::FindSmartObjectsWithInteractionEntranceInActor(
	const FSmartObjectRequestFilter& Filter, AActor* SearchActor, TArray<FSmartObjectRequestResult>& OutResults,
	const AActor* UserActor)
{
	if (!IsValid(SearchActor))
	{
		UE_LOG( LogTemp, Warning, TEXT("SearchActor is not valid"));
		return false;
	}
	TArray<FSmartObjectRequestResult> Results;
	USmartObjectBlueprintFunctionLibrary::FindSmartObjectsInActor(Filter, SearchActor, Results, UserActor);
	if (Results.IsEmpty())
	{
		UE_LOG( LogTemp, Warning, TEXT("No smart objects found in actor %s"), *SearchActor->GetName());
		return false;
	}

	// has the results our definition entry?
	for (int32 i = 0; i < Results.Num(); i++)
	{
		UInt_InteractionDefinition* FoundDefinition;
		if (FindInteractionDefinitionFromSmartObjectSlot(SearchActor, Results[i].SlotHandle, FoundDefinition))
		{
			OutResults.Add(Results[i]);
		}
	}
	return !OutResults.IsEmpty();
}

bool UInt_SmartObjectFunctionLibrary::FindInteractionDefinitionFromSmartObjectSlot(UObject* WorldContext,
	FSmartObjectSlotHandle SmartObjectSlotHandle, UInt_InteractionDefinition*& OutDefinition)
{
	bool bFound = false;

	if (WorldContext && WorldContext->GetWorld() && SmartObjectSlotHandle.IsValid())
	{
		if (USmartObjectSubsystem* SO_Subsystem = WorldContext->GetWorld()->GetSubsystem<USmartObjectSubsystem>())
		{
			SO_Subsystem->ReadSlotData(SmartObjectSlotHandle, [&OutDefinition, &bFound](FConstSmartObjectSlotView SlotView)
			{
				if (SlotView.IsValid() && SlotView.GetSlotHandle().IsValid())
				{
					if (const FInt_SmartObjectInteractionEntranceData* Entry = SlotView.GetDefinitionDataPtr<FInt_SmartObjectInteractionEntranceData>())
					{
						if (!Entry->DefinitionDA.IsNull())
						{
							UE_LOG(LogTemp, Warning, TEXT("Loading interaction definition from data asset"));
							OutDefinition = Entry->DefinitionDA.LoadSynchronous();
							bFound = true;
						}
					}
				}
			});
		}
	}
	return bFound;
}


