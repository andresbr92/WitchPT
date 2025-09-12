// Fill out your copyright notice in the Description page of Project Settings.

#include "Int_InteractableInterface.h"
// Add default functionality here for any IInt_InteractableInterface functions that are not pure virtual.
FText IInt_InteractableInterface::GetInteractionDisplayName_Implementation() const
{
	if (UObject* Object = _getUObject())
	{
		return FText::FromString(GetNameSafe(Object));
	}
	return FText::GetEmpty();
}