// Copyright Epic Games, Inc. All Rights Reserved.

#include "AndresD3v_UIManagerSystem.h"

#define LOCTEXT_NAMESPACE "FAndresD3v_UIManagerSystemModule"

void FAndresD3v_UIManagerSystemModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FAndresD3v_UIManagerSystemModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAndresD3v_UIManagerSystemModule, AndresD3v_UIManagerSystem)