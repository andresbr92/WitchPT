// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WitchPT_GameUIContextBase.h"
#include "CauldronUIContext.generated.h"

class ACauldronAltar;
/**
 * 
 */
USTRUCT()
struct FUICauldronUIContext
{
	GENERATED_BODY()
	
};
UCLASS()
class WITCHPT_API UCauldronUIContext : public UWitchPT_GameUIContextBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn = "true"))
	TObjectPtr<ACauldronAltar> CauldronAltar;
};
