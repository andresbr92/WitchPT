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
    
    // Try to find the CauldronAltar if it wasn't set in the editor
    if (!CauldronAltar)
    {
        FindCauldronAltar();
    }
}

void ACauldronPosition::FindCauldronAltar()
{
    // First check if we are attached to an altar
    AActor* ParentActor = GetAttachParentActor();
    if (ParentActor)
    {
        ACauldronAltar* PotentialAltar = Cast<ACauldronAltar>(ParentActor);
        if (PotentialAltar)
        {
            CauldronAltar = PotentialAltar;
            UE_LOG(LogTemp, Log, TEXT("[CauldronPosition] %s found attached CauldronAltar %s"), 
                *GetName(), *CauldronAltar->GetName());
            return;
        }
    }
    
    // If not attached, try to find any altar in the world
    TArray<AActor*> FoundAltars;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACauldronAltar::StaticClass(), FoundAltars);
    
    if (FoundAltars.Num() > 0)
    {
        CauldronAltar = Cast<ACauldronAltar>(FoundAltars[0]);
        UE_LOG(LogTemp, Log, TEXT("[CauldronPosition] %s found world CauldronAltar %s"), 
            *GetName(), *CauldronAltar->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("[CauldronPosition] %s couldn't find a CauldronAltar!"), *GetName());
    }
}

void ACauldronPosition::GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& OptionBuilder)
{
    Super::GatherInteractionOptions(InteractQuery, OptionBuilder);
    
    // // Cauldron-specific interaction options
    //
    // ACharacter* InteractingCharacterPtr = Cast<ACharacter>(InteractQuery.User);
    //
    // // If no player is occupying the position, let a player take the position
    // if (!IsOccupied() && InteractingCharacterPtr)
    // {
    //     OptionBuilder.AddDefaultInteraction(
    //         FText::FromString("Take Position"), // Text for the interaction
    //         [this, InteractingCharacterPtr](const FInteractionExecuteContext& Context)
    //         {
    //             HandleInteraction(InteractingCharacterPtr);
    //         }
    //     );
    // }
    //
    // // Special case: if the character is standing at a position and we're in Inactive state, they can initiate brewing
    // if (IsOccupied() && CauldronAltar && GetOccupyingCharacter() == InteractingCharacterPtr)
    // {
    //     // Check cauldron state
    //     if (CauldronAltar->GetCurrentState() == EInteractionState::Inactive)
    //     {
    //         OptionBuilder.AddDefaultInteraction(
    //             FText::FromString("Start Brewing"), // Text for the interaction
    //             [this, InteractingCharacterPtr](const FInteractionExecuteContext& Context)
    //             {
    //                 if (CauldronAltar)
    //                 {
    //                     // Cast to the appropriate start brewing function
    //                     CauldronAltar->StartBrewing(InteractingCharacterPtr);
    //                 }
    //             }
    //         );
    //     }
    // }
    //
    // // If this player is at a position and brewing is active, they can add ingredients
    // if (IsOccupied() && CauldronAltar && GetOccupyingCharacter() == InteractingCharacterPtr)
    // {
    //     if (CauldronAltar->GetCurrentState() == EInteractionState::Active)
    //     {
    //         // Get the player's current expected input
    //         FGameplayTag ExpectedInput = CauldronAltar->GetCurrentExpectedInputForPlayer(InteractingCharacterPtr);
    //         
    //         if (ExpectedInput.IsValid())
    //         {
    //             // Determine the ingredient name
    //             FString TagString = ExpectedInput.ToString();
    //             FString IngredientName = "Unknown Ingredient";
    //             
    //             if (TagString.Contains("Ingredient1"))
    //             {
    //                 IngredientName = "Red Mushroom";
    //             }
    //             else if (TagString.Contains("Ingredient2"))
    //             {
    //                 IngredientName = "Frog Leg";
    //             }
    //             else if (TagString.Contains("Ingredient3"))
    //             {
    //                 IngredientName = "Bat Wing";
    //             }
    //             else if (TagString.Contains("Ingredient4"))
    //             {
    //                 IngredientName = "Snake Venom";
    //             }
    //             else if (TagString.Contains("Ingredient5"))
    //             {
    //                 IngredientName = "Dragon Scale";
    //             }
    //             
    //             // Give the player the option to add the correct ingredient
    //             OptionBuilder.AddDefaultInteraction(
    //                 FText::FromString(FString::Printf(TEXT("Add %s"), *IngredientName)),
    //                 [this, InteractingCharacterPtr, ExpectedInput](const FInteractionExecuteContext& Context)
    //                 {
    //                     if (CauldronAltar)
    //                     {
    //                         CauldronAltar->HandlePlayerInput(InteractingCharacterPtr, ExpectedInput);
    //                     }
    //                 }
    //             );
    //             
    //             // Also add wrong ingredient options for fun/challenge
    //             for (int32 i = 1; i <= 5; ++i)
    //             {
    //                 FString WrongTagString = FString::Printf(TEXT("Input.Cauldron.Ingredient%d"), i);
    //                 FGameplayTag WrongTag = FGameplayTag::RequestGameplayTag(FName(*WrongTagString));
    //                 
    //                 // Skip if this is the correct ingredient
    //                 if (WrongTag == ExpectedInput)
    //                 {
    //                     continue;
    //                 }
    //                 
    //                 // Determine wrong ingredient name
    //                 FString WrongIngredientName = "Unknown Ingredient";
    //                 switch (i)
    //                 {
    //                 case 1: WrongIngredientName = "Red Mushroom"; break;
    //                 case 2: WrongIngredientName = "Frog Leg"; break;
    //                 case 3: WrongIngredientName = "Bat Wing"; break;
    //                 case 4: WrongIngredientName = "Snake Venom"; break;
    //                 case 5: WrongIngredientName = "Dragon Scale"; break;
    //                 }
    //                 
    //                 OptionBuilder.AddDefaultInteraction(
    //                     FText::FromString(FString::Printf(TEXT("Add %s"), *WrongIngredientName)),
    //                     [this, InteractingCharacterPtr, WrongTag](const FInteractionExecuteContext& Context)
    //                     {
    //                         if (CauldronAltar)
    //                         {
    //                             CauldronAltar->HandlePlayerInput(InteractingCharacterPtr, WrongTag);
    //                         }
    //                     }
    //                 );
    //             }
    //         }
    //     }
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
    CauldronAltar->OccupyPosition(InteractingCharacter, this);
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