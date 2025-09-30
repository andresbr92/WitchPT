// Fill out your copyright notice in the Description page of Project Settings.


#include "Filter/Int_TargetingFilterTask_InteractionSmartObjects.h"

#include "Int_SmartObjectFunctionLibrary.h"
#include "Components/Int_InteractionSystemComponent.h"

bool UInt_TargetingFilterTask_InteractionSmartObjects::ShouldFilterTarget(
	const FTargetingRequestHandle& TargetingHandle, const FTargetingDefaultResultData& TargetData) const
{
	if (const FTargetingSourceContext* SourceContext = FTargetingSourceContext::Find(TargetingHandle))
	{
		if (AActor* Actor = TargetData.HitResult.GetActor())
		{
			if (UInt_InteractionSystemComponent* InteractionSys = UInt_InteractionSystemComponent::GetInteractionSystemComponent(SourceContext->SourceActor))
			{
				TArray<FSmartObjectRequestResult> Results;

				return !UInt_SmartObjectFunctionLibrary::FindSmartObjectsWithInteractionEntranceInActor(InteractionSys->GetSmartObjectRequestFilter(), Actor, Results, InteractionSys->GetOwner());
			}
		}
	}
	return true;
}
