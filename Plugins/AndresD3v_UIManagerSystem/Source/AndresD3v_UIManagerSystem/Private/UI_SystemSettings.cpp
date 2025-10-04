// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_SystemSettings.h"

const UUI_SystemSettings* UUI_SystemSettings::Get()
{
	return GetDefault<UUI_SystemSettings>();
}
