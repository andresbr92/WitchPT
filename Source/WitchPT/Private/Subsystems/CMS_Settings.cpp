// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/CMS_Settings.h"

const UCMS_Settings* UCMS_Settings::Get()
{
	return GetDefault<UCMS_Settings>();
}
