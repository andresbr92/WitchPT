#include "Item/CauldronPosition.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Item/CauldronAltar.h" // Include the Altar header
#include "Kismet/GameplayStatics.h"

ACauldronPosition::ACauldronPosition()
{
    // No need to set up replication here - it's handled by the base class
    bIsOccupied = false;
    OccupyingCharacter = nullptr;
}

void ACauldronPosition::BeginPlay()
{
    Super::BeginPlay();
    
    // // Try to find the CauldronAltar if it wasn't set in the editor
    // if (!CauldronAltar)
    // {
    //     FindCauldronAltar();
    // }
}

void ACauldronPosition::FindCauldronAltar()
{
    // First check if we are attached to an altar
    // AActor* ParentActor = GetAttachParentActor();
    // if (ParentActor)
    // {
    //     ACauldronAltar* PotentialAltar = Cast<ACauldronAltar>(ParentActor);
    //     if (PotentialAltar)
    //     {
    //         CauldronAltar = PotentialAltar;
    //         UE_LOG(LogTemp, Log, TEXT("[CauldronPosition] %s found attached CauldronAltar %s"), 
    //             *GetName(), *CauldronAltar->GetName());
    //         return;
    //     }
    // }
    //
    // // If not attached, try to find any altar in the world
    // TArray<AActor*> FoundAltars;
    // UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACauldronAltar::StaticClass(), FoundAltars);
    //
    // if (FoundAltars.Num() > 0)
    // {
    //     CauldronAltar = Cast<ACauldronAltar>(FoundAltars[0]);
    //     UE_LOG(LogTemp, Log, TEXT("[CauldronPosition] %s found world CauldronAltar %s"), 
    //         *GetName(), *CauldronAltar->GetName());
    // }
    // else
    // {
    //     UE_LOG(LogTemp, Warning, TEXT("[CauldronPosition] %s couldn't find a CauldronAltar!"), *GetName());
    // }
}



void ACauldronPosition::HandleInteraction(ACharacter* InteractingCharacter)
{
    Super::HandleInteraction(InteractingCharacter);
    
    if (!InteractingCharacter || !CauldronAltar)
    {
        return;
    }
    
    // Call the cauldron altar to handle occupation
    // CauldronAltar->OccupyPosition(InteractingCharacter, this);
}

// bool ACauldronPosition::PositionCharacter(ACharacter* Character)
// {
//     if (!Character)
//     {
//         UE_LOG(LogTemp, Warning, TEXT("ACauldronPosition::PositionCharacter: Invalid character"));
//         return false;
//     }
//     
//     // No permitir posicionar si ya está ocupado
//     if (bIsOccupied && OccupyingCharacter != Character)
//     {
//         UE_LOG(LogTemp, Warning, TEXT("ACauldronPosition::PositionCharacter: Position already occupied"));
//         return false;
//     }
//     
//     // Teleportar al personaje a la ubicación de este actor
//     FVector TargetLocation = GetActorLocation();
//     FRotator TargetRotation = GetActorRotation();
//     
//     bool bSuccess = Character->SetActorLocationAndRotation(
//         TargetLocation, 
//         TargetRotation, 
//         false, 
//         nullptr, 
//         ETeleportType::TeleportPhysics);
//     
//     if (bSuccess)
//     {
//         // Marcar esta posición como ocupada
//         SetOccupied(true, Character);
//         
//         UE_LOG(LogTemp, Log, TEXT("ACauldronPosition::PositionCharacter: Character positioned at %s"), 
//                *GetName());
//     }
//     
//     return bSuccess;
// }
//
// bool ACauldronPosition::IsOccupied() const
// {
//     return bIsOccupied && OccupyingCharacter != nullptr;
// }
//
// void ACauldronPosition::SetOccupied(bool bInOccupied, ACharacter* InOccupyingCharacter)
// {
//     bIsOccupied = bInOccupied;
//     
//     if (bInOccupied)
//     {
//         OccupyingCharacter = InOccupyingCharacter;
//     }
//     else
//     {
//         OccupyingCharacter = nullptr;
//     }
//     
//     UE_LOG(LogTemp, Log, TEXT("ACauldronPosition::SetOccupied: %s is now %s"), 
//            *GetName(), bIsOccupied ? TEXT("occupied") : TEXT("unoccupied"));
// } 