// Copyright Epic Games, Inc. All Rights Reserved.

#include "AndresD3v_InventorySystem.h"

#define LOCTEXT_NAMESPACE "FAndresD3v_InteractionSystemModule"

void FAndresD3v_InventorySystemModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FAndresD3v_InventorySystemModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAndresD3v_InventorySystemModule, AndresD3v_InventorySystem)