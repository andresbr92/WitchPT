// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#define ECC_Projectile ECollisionChannel::ECC_GameTraceChannel1;
#define WitchPT_TraceChannel_Interaction ECC_GameTraceChannel2
#define CONSOLE_LOG(Format, ...) UE_LOG(LogTemp, Warning, TEXT("%s: " Format), *FString(__FUNCTION__), ##__VA_ARGS__)

