// Fill out your copyright notice in the Description page of Project Settings.


#include "WitchPTIndicatorManagerComponent.h"

#include "IndicatorDescriptor.h"


// Sets default values for this component's properties
UWitchPTIndicatorManagerComponent::UWitchPTIndicatorManagerComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	bAutoRegister = true;
	bAutoActivate = true;
}

UWitchPTIndicatorManagerComponent* UWitchPTIndicatorManagerComponent::GetComponent(AController* Controller)
{
	if (Controller)
	{
		return Controller->FindComponentByClass<UWitchPTIndicatorManagerComponent>();
	}

	return nullptr;
}

void UWitchPTIndicatorManagerComponent::AddIndicator(UIndicatorDescriptor* IndicatorDescriptor)
{
	IndicatorDescriptor->SetIndicatorManagerComponent(this);
	OnIndicatorAdded.Broadcast(IndicatorDescriptor);
	Indicators.Add(IndicatorDescriptor);
}

void UWitchPTIndicatorManagerComponent::RemoveIndicator(UIndicatorDescriptor* IndicatorDescriptor)
{
	if (IndicatorDescriptor)
	{
		ensure(IndicatorDescriptor->GetIndicatorManagerComponent() == this);
	
		OnIndicatorRemoved.Broadcast(IndicatorDescriptor);
		Indicators.Remove(IndicatorDescriptor);
	}
}




