// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/WitchPTEquipmentInstance.h"

#include "Equipment/WitchPTEquipmentDefinition.h"
#include "GameFramework/Character.h"

UWitchPTEquipmentInstance::UWitchPTEquipmentInstance(const FObjectInitializer& ObjectInitializer)
{
}

UWorld* UWitchPTEquipmentInstance::GetWorld() const
{
	if (const APawn* OwningPawn = GetPawn())
	{
		return OwningPawn->GetWorld();
	}
	return nullptr;
}

void UWitchPTEquipmentInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, Instigator);
}

void UWitchPTEquipmentInstance::SpawnEquipmentActors(const TArray<FWitchPTEquipmentActorToSpawn>& ActorsToSpawn)
{
	if (APawn* OwningPawn = GetPawn())
	{
		USceneComponent* AttachTarget = OwningPawn->GetRootComponent();
		if (ACharacter* Char = Cast<ACharacter>(OwningPawn))
		{
			AttachTarget = Char->GetMesh();
		}

		for (const FWitchPTEquipmentActorToSpawn& SpawnInfo : ActorsToSpawn)
		{
			AActor* NewActor = GetWorld()->SpawnActorDeferred<AActor>(SpawnInfo.ActorToSpawn, FTransform::Identity, OwningPawn);
			NewActor->FinishSpawning(FTransform::Identity, /*bIsDefaultTransform=*/ true);
			NewActor->SetActorRelativeTransform(SpawnInfo.AttachTransform);
			NewActor->AttachToComponent(AttachTarget, FAttachmentTransformRules::KeepRelativeTransform, SpawnInfo.AttachSocket);

			SpawnedActors.Add(NewActor);
		}
	}
}

void UWitchPTEquipmentInstance::DestroyEquipmentActors()
{
	for (AActor* Actor : SpawnedActors)
	{
		if (Actor)
		{
			Actor->Destroy();
		}
	}
}

void UWitchPTEquipmentInstance::OnEquipped()
{
	UE_LOG(LogTemp, Warning, TEXT("✅ EQUIPPED: %s"), *GetClass()->GetName());
	K2_OnEquipped();
}

void UWitchPTEquipmentInstance::OnUnequipped()
{
	UE_LOG(LogTemp, Warning, TEXT("❌ UNEQUIPPED: %s"), *GetClass()->GetName());
	K2_OnUnequipped();
}

APawn* UWitchPTEquipmentInstance::GetPawn() const
{
	return Cast<APawn>(GetOuter());
}

void UWitchPTEquipmentInstance::OnRep_Instigator()
{
}
