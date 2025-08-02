// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GenericUISystemSettings.h"

const UGenericUISystemSettings* UGenericUISystemSettings::Get()
{
	return GetDefault<UGenericUISystemSettings>();
}
