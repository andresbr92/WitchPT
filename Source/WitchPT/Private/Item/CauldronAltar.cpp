#include "Item/CauldronAltar.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Item/CauldronPosition.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "FWitchPTGameplayTags.h"
#include "Inventory/WitchPTInventoryItemInstance.h"
#include "Inventory/Fragments/WitchPTInventoryItemFragment_IngredientDetails.h"
#include "Player/WitchPTPlayerController.h"

// Sets default values
ACauldronAltar::ACauldronAltar()
{
    // Set this actor to call Tick() every frame
    PrimaryActorTick.bCanEverTick = true;

    // Make sure it replicates
    bReplicates = true;
    bAlwaysRelevant = true;
    CauldronPhysicState = ECauldronPhysicState::Static;
    CarryingCharacter = nullptr;
    CurrentPlacementState = ECauldronPlacementState::None;
    
}

void ACauldronAltar::GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& OptionBuilder)
{
    // Set up the interaction option based on the cauldron state
    FInteractionOption InteractionOption = Option;
    
    // Configure the interaction option to support hold interaction
    InteractionOption.bSupportsHoldInteraction = true;
    
    // Add the interaction option to the builder
    OptionBuilder.AddInteractionOption(InteractionOption);
}

void ACauldronAltar::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    DOREPLIFETIME(ACauldronAltar, CauldronPhysicState);
    DOREPLIFETIME(ACauldronAltar, CarryingCharacter);
    DOREPLIFETIME(ACauldronAltar, CurrentPlacementState);
    DOREPLIFETIME(ACauldronAltar, BaseIngredient);
}


void ACauldronAltar::OnRep_CauldronPhysicState()
{
    // Update visual representation based on the new state
    if (CauldronPhysicState == ECauldronPhysicState::Moving)
    {
        // Cauldron is being carried - update visuals if needed
        SetActorEnableCollision(false);
    }
    else if (CauldronPhysicState == ECauldronPhysicState::Previewing)
    {
        // Cauldron is in preview mode - update visuals
        SetActorEnableCollision(false);
        
        
    }
    
}

void ACauldronAltar::OnRep_BaseIngredient()
{
    BroadcastBaseIngredientDropped();
}

// --- Interaction Functions ---

void ACauldronAltar::StartBrewingPotion(ACharacter* InteractingCharacter)
{
    if (!InteractingCharacter)
    {
        UE_LOG(LogTemp, Warning, TEXT("ACauldronAltar::StartBrewingPotion: Invalid character"));
        return;
    }
    
    // Si el caldero está en modo de previsualización, finalizar la colocación
    if (IsInPlacementPreview() && CarryingCharacter == InteractingCharacter)
    {
        FinalizePlacement();
        return;
    }
    
    // Otherwise, try to position the character for brewing
    PositionCharacterForBrewing(InteractingCharacter);
}

void ACauldronAltar::RequestDropBaseIngredient(ACharacter* RequestingCharacter,UWitchPTInventoryItemInstance* IngredientInstance)
{
    if (!HasAuthority())
    {
        UE_LOG(LogTemp, Warning, TEXT("ACauldronAltar::RequestDropBaseIngredient: Not authority"));
        return;
    }
    if (!IngredientInstance)
    {
        UE_LOG(LogTemp, Warning, TEXT("ACauldronAltar::RequestDropBaseIngredient: Invalid ingredient instance"));
        return;
    }
    // Check if the cauldron is in a valid state to drop an ingredient
    if (CauldronPhysicState != ECauldronPhysicState::Static)
    {
        UE_LOG(LogTemp, Warning, TEXT("ACauldronAltar::RequestDropBaseIngredient: Cauldron is not in a static state"));
        return;
    }
    const UWitchPTInventoryItemFragment* ItemFragment = IngredientInstance->FindFragmentByClass(UWitchPTInventoryItemFragment_IngredientDetails::StaticClass());
    BaseIngredient = IngredientInstance;
    // Marcar la propiedad como dirty para replicación
    // Cast<UWitchPTInventoryItemFragment_IngredientDetails>(ItemFragment);

    BroadcastBaseIngredientDropped();
    

   
}

void ACauldronAltar::BeginPlay()
{
    Super::BeginPlay();
    SetReplicateMovement(true);
}

void ACauldronAltar::StartCarryCauldron(ACharacter* InteractingCharacter)
{
    if (!InteractingCharacter)
    {
        UE_LOG(LogTemp, Warning, TEXT("ACauldronAltar::StartCarryCauldron: Invalid character"));
        return;
    }
    
    // Si el caldero está en modo de previsualización, cancelar y volver a adjuntar al personaje
    if (IsInPlacementPreview() && CarryingCharacter == InteractingCharacter)
    {
        CancelPlacement();
        return;
    }
    
    // Check if the cauldron can be picked up
    if (!CanBePickedUp())
    {
        UE_LOG(LogTemp, Warning, TEXT("ACauldronAltar::StartCarryCauldron: Cauldron cannot be picked up"));
        return;
    }
    
    // Attach the cauldron to the character
    AttachToCharacter(InteractingCharacter);
}

bool ACauldronAltar::CanBePickedUp() const
{
    // Check if the cauldron is already being carried
    if (IsBeingCarried() || IsInPlacementPreview())
    {
        return false;
    }
    
    // Add any additional conditions here (e.g., cauldron is empty, not brewing, etc.)
    
    return true;
}

bool ACauldronAltar::IsBeingCarried() const
{
    return CauldronPhysicState == ECauldronPhysicState::Moving && CarryingCharacter != nullptr;
}

bool ACauldronAltar::IsInPlacementPreview() const
{
    return CauldronPhysicState == ECauldronPhysicState::Previewing && CarryingCharacter != nullptr;
}


ACharacter* ACauldronAltar::GetCarryingCharacter() const
{
    return CarryingCharacter;
}

void ACauldronAltar::AttachToCharacter(ACharacter* Character)
{
    if (!Character || !HasAuthority())
    {
        return;
    }
    
    // First destroy any brewing positions
    DestroyAltarPositions();
    
    // Set the cauldron state to moving
    CauldronPhysicState = ECauldronPhysicState::Moving;
    CarryingCharacter = Character;
    
    // Attach the cauldron to the character's back
    FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, 
                                          EAttachmentRule::KeepWorld, true);
    
    // Find the socket to attach to
    USkeletalMeshComponent* CharacterMesh = Character->GetMesh();
    if (CharacterMesh && CharacterMesh->DoesSocketExist(BackAttachSocketName))
    {
        // Attach to the socket
        AttachToComponent(CharacterMesh, AttachRules, BackAttachSocketName);
        
        // Disable collision
        SetActorEnableCollision(false);
        
        // Reduce de Scale of the Cauldron
        SetActorScale3D(FVector(0.5f, 0.5f, 0.5f));
        
        UE_LOG(LogTemp, Log, TEXT("ACauldronAltar::AttachToCharacter: Cauldron attached to %s"), *Character->GetName());
    }
    
    // Play pickup sound or effects
    // PlayPickupEffects();
}

void ACauldronAltar::DetachFromCharacter(ACharacter* Character)
{
    if (!Character || !HasAuthority() || Character != CarryingCharacter)
    {
        return;
    }
    
    // Detach from the character
    DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    
    // Place the cauldron in front of the character
    FVector CharacterLocation = Character->GetActorLocation();
    FRotator CharacterRotation = Character->GetActorRotation();
    FVector PlacementOffset = CharacterRotation.RotateVector(DetachmentOffset);
    
    // Set the cauldron's location
    SetActorLocation(CharacterLocation + PlacementOffset);
    
    // Reset the cauldron state
    CauldronPhysicState = ECauldronPhysicState::Static;
    CarryingCharacter = nullptr;
    
    // Reset cauldron scale to normal
    SetActorScale3D(FVector(1.0f, 1.0f, 1.0f));
    
    // Enable collision
    SetActorEnableCollision(true);
    
    // Create brewing positions around the cauldron
    CreateAltarPositions();
    
    UE_LOG(LogTemp, Log, TEXT("ACauldronAltar::DetachFromCharacter: Cauldron detached from %s"), *Character->GetName());
    
    // Play placement sound or effects
    // PlayPlacementEffects();
}

void ACauldronAltar::PositionCharacterForBrewing(ACharacter* Character)
{
    if (!Character || !HasAuthority())
    {
        UE_LOG(LogTemp, Warning, TEXT("ACauldronAltar::PositionCharacterForBrewing: Invalid character"));
        return;
    }
    
    // Obtener una posición disponible
    ABaseInteractionPosition* BrewingPosition = GetAvailableBrewingPosition(Character);
    
    if (BrewingPosition)
    {
        Character->SetActorLocationAndRotation(BrewingPosition->GetActorLocation(), BrewingPosition->GetActorRotation(), false, nullptr, ETeleportType::TeleportPhysics);
        ParticipatingPlayers.Add(Character);
        
        if (Character->GetLocalRole() == ROLE_Authority && Character->IsLocallyControlled()) // Im the listen server
        {
            
            if (AWitchPTPlayerController* PC = Cast<AWitchPTPlayerController>(Character->GetOwner()))
            {
                PC->LocalToggleCauldronMenu();
            }
        } else if (Character->HasAuthority() && !Character->IsLocallyControlled()) // The call is from the client
        {
           
            if ( AWitchPTPlayerController* PC = Cast<AWitchPTPlayerController>(Character->GetOwner()))
            {
                PC->Client_ToggleCauldronMenu();
            }
        }
        FGameplayEventData Payload;
        Payload.OptionalObject = Character;
        FGameplayTag GameplayTag = FWitchPTGameplayTags::Get().Event_Cauldron_CharacterPositioned;
        UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Character, GameplayTag, Payload);
        return BrewingPosition->SetOccupied(Character);

        // Teleport the character to the position facing the cauldron
        

    }
  

    

    // Call the ritual state delegate for Listen Server
  
}


ABaseInteractionPosition* ACauldronAltar::GetAvailableBrewingPosition(ACharacter* Character)
{
    // Verificar que tengamos posiciones
    if (InteractionPositions.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("ACauldronAltar::GetAvailableBrewingPosition: No brewing positions available"));
        return nullptr;
    }
    
    // Buscar la primera posición disponible
    for (ABaseInteractionPosition* Position : InteractionPositions)
    {
        if (Position && !Position->IsOccupied())
        {
            return Position;
        }
    }
    
    // Todas las posiciones están ocupadas
    UE_LOG(LogTemp, Warning, TEXT("ACauldronAltar::GetAvailableBrewingPosition: All positions are occupied"));
    
    return nullptr;
}

// --- Nueva implementación para colocación del caldero ---

void ACauldronAltar::StartPlacementPreview(ACharacter* Character)
{
    if (!Character || Character != CarryingCharacter || !HasAuthority())
    {
        return;
    }
    
    // Desacoplar el caldero del personaje
    DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    
    // Establecer el estado de previsualización
    CauldronPhysicState = ECauldronPhysicState::Previewing;
    
    // Guardar los materiales originales para restaurarlos después
    UStaticMeshComponent* MeshComponent = FindComponentByClass<UStaticMeshComponent>();
    if (MeshComponent)
    {
        OriginalMaterials.Empty();
        for (int32 i = 0; i < MeshComponent->GetNumMaterials(); i++)
        {
            OriginalMaterials.Add(MeshComponent->GetMaterial(i));
        }
    }
    
    // Aplicar material de previsualización
    CurrentPlacementState = ECauldronPlacementState::Invalid; // Por defecto, inválido hasta que se verifique
    // ApplyPlacementPreviewMaterial();
    
    // Desactivar colisiones durante la previsualización
    SetActorEnableCollision(false);
    
    // Colocar inicialmente el caldero frente al personaje
    FVector CharacterLocation = Character->GetActorLocation();
    FRotator CharacterRotation = Character->GetActorRotation();
    FVector PlacementOffset = CharacterRotation.RotateVector(DetachmentOffset);

    // Guardar la posición inicial
    PreviewLocation = CharacterLocation + PlacementOffset;
    // PreviewRotation = CharacterRotation;
    // Reset z rotation
    PreviewRotation.Pitch = 0;
    PreviewRotation.Roll = 0;
    PreviewRotation.Yaw = 0;
    // Establecer la posición del caldero
    SetActorLocation(PreviewLocation);
    SetActorRotation(PreviewRotation);
    
    // Restablecer la escala normal
    Multicast_FinalizePlacement();
    
    
}

void ACauldronAltar::Client_UpdatePlacementPreview_Implementation(const FVector& HitLocation, const FVector& HitNormal)
{
   
    
    // Actualizar el material según el estado
    // ApplyPlacementPreviewMaterial();
}

void ACauldronAltar::UpdatePlacementPreview(const FVector& HitLocation, const FVector& HitNormal)
{
    // Llamar a la implementación del cliente para actualizar la previsualización
    // Client_UpdatePlacementPreview(HitLocation, HitNormal);
    if (!IsInPlacementPreview())
    {
        return;
    }
    
    // Guardar la nueva posición de previsualización
    PreviewLocation = HitLocation;
    
    // Calcular la rotación para alinear con la superficie
    FRotator SurfaceAlignedRotation = HitNormal.Rotation();
    
    // Ajustar la rotación para que el caldero se alinee con la superficie pero mantenga su orientación "hacia arriba"
    // Necesitamos solo el componente de inclinación de la superficie
    FRotator AdjustedRotation = FRotator(SurfaceAlignedRotation.Pitch, PreviewRotation.Yaw, SurfaceAlignedRotation.Roll);
    
    // Limitar la inclinación máxima
    if (FMath::Abs(AdjustedRotation.Pitch) > 15.0f)
    {
        AdjustedRotation.Pitch = FMath::Clamp(AdjustedRotation.Pitch, 0.f, 0.f);
    }
    
    if (FMath::Abs(AdjustedRotation.Roll) > 15.0f)
    {
        AdjustedRotation.Roll = FMath::Clamp(AdjustedRotation.Roll, 0.f, 0.f);
    }
    
    PreviewRotation = AdjustedRotation;
    SetActorScale3D(FVector(1.0f, 1.0f, 1.0f));
    SetActorLocation(PreviewLocation);
    
    
    // Verificar si la posición es válida
    CurrentPlacementState = IsPlacementValid() ? ECauldronPlacementState::Valid : ECauldronPlacementState::Invalid;
}



void ACauldronAltar::FinalizePlacement()
{
    if (!IsInPlacementPreview() || !HasAuthority())
    {
        return;
    }
    
    // Establecer la posición final
    SetActorScale3D(FVector(1.0f, 1.0f, 1.0f));
    SetActorLocation(PreviewLocation);
    SetActorRotation(PreviewRotation);
    
    // Restaurar los materiales originales
    RestoreOriginalMaterials();
    
    // Activar colisiones
    SetActorEnableCollision(true);
    
    // Cambiar el estado
    CauldronPhysicState = ECauldronPhysicState::Static;
    CarryingCharacter = nullptr;
    CurrentPlacementState = ECauldronPlacementState::None;
    
    OnECauldronPhysicStateChanged.Broadcast(ECauldronPhysicState::Static);
    
    // Crear posiciones de elaboración alrededor del caldero
    CreateAltarPositions();

    // Multicast finalizar placement
    Multicast_FinalizePlacement();

    UE_LOG(LogTemp, Log, TEXT("ACauldronAltar::FinalizePlacement: Cauldron placed successfully"));
}

void ACauldronAltar::CancelPlacement()
{
    if (!IsInPlacementPreview() || !HasAuthority())
    {
        return;
    }
    
    // Restaurar materiales originales
    RestoreOriginalMaterials();
    
    // Recolocar el caldero en el personaje
    ACharacter* Character = CarryingCharacter;
    
    // Reiniciar estado
    CauldronPhysicState = ECauldronPhysicState::Moving;
    CarryingCharacter = nullptr;
    CurrentPlacementState = ECauldronPlacementState::None;
    
    // Volver a adjuntar el caldero al personaje
    if (Character)
    {
        AttachToCharacter(Character);
    }
    
    UE_LOG(LogTemp, Log, TEXT("ACauldronAltar::CancelPlacement: Placement canceled"));
}

void ACauldronAltar::UnoccupyPosition(ACharacter* Character, ABaseInteractionPosition* Position)
{
    // Find the position occupied by the player
    for(ABaseInteractionPosition* PositionOccupied : InteractionPositions)
    {
        if(PositionOccupied->GetOccupyingCharacter() == Character)
        {
            // Close the cauldron menu
             if (Character->GetLocalRole() == ROLE_Authority && Character->IsLocallyControlled()) // Im the listen server
        {
            
            if (AWitchPTPlayerController* PC = Cast<AWitchPTPlayerController>(Character->GetOwner()))
            {
                PC->LocalToggleCauldronMenu();
                
            }
        } else if (Character->HasAuthority() && !Character->IsLocallyControlled()) // The call is from the client
        {
           
            if ( AWitchPTPlayerController* PC = Cast<AWitchPTPlayerController>(Character->GetOwner()))
            {
                PC->Client_ToggleCauldronMenu();
                
            }
        }
            PositionOccupied->SetOccupied(nullptr);
            break;
        }
    }
    
}




void ACauldronAltar::Multicast_FinalizePlacement_Implementation()
{
    // Reset de scale for all clients
    SetActorScale3D(FVector(1.0f, 1.0f, 1.0f));
    SetActorEnableCollision(true);
}

ECauldronPlacementState ACauldronAltar::GetPlacementState() const
{
    return CurrentPlacementState;
}

void ACauldronAltar::ApplyPlacementPreviewMaterial()
{
    UStaticMeshComponent* MeshComponent = FindComponentByClass<UStaticMeshComponent>();
    if (!MeshComponent)
    {
        return;
    }
    
    // Determinar qué material aplicar según el estado
    UMaterialInterface* MaterialToApply = CurrentPlacementState == ECauldronPlacementState::Valid ? 
        ValidPlacementMaterial : InvalidPlacementMaterial;
    
    // Si no tenemos un material específico, no hacemos nada
    if (!MaterialToApply)
    {
        return;
    }
    
    // Aplicar el material a todos los elementos del mesh
    for (int32 i = 0; i < MeshComponent->GetNumMaterials(); i++)
    {
        MeshComponent->SetMaterial(i, MaterialToApply);
    }
}

void ACauldronAltar::RestoreOriginalMaterials()
{
    // Restaurar los materiales originales si estamos fuera del modo de previsualización
    if (CauldronPhysicState != ECauldronPhysicState::Previewing)
    {
        UStaticMeshComponent* MeshComponent = FindComponentByClass<UStaticMeshComponent>();
        if (MeshComponent && OriginalMaterials.Num() > 0)
        {
            for (int32 i = 0; i < FMath::Min(MeshComponent->GetNumMaterials(), OriginalMaterials.Num()); i++)
            {
                if (OriginalMaterials[i])
                {
                    MeshComponent->SetMaterial(i, OriginalMaterials[i]);
                }
            }
        }
    }
}

bool ACauldronAltar::IsPlacementValid() const
{
    if (!GetWorld())
    {
        return false;
    }
    
    // 1. Comprobar si hay objetos en el radio de colisión
    TArray<FOverlapResult> Overlaps;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);
    
    if (CarryingCharacter)
    {
        QueryParams.AddIgnoredActor(CarryingCharacter);
    }
    
    bool bHasOverlaps = GetWorld()->OverlapMultiByObjectType(
        Overlaps,
        PreviewLocation,
        FQuat::Identity,
        FCollisionObjectQueryParams::AllObjects,
        FCollisionShape::MakeSphere(PlacementCollisionCheckRadius),
        QueryParams
    );
    
    if (bHasOverlaps)
    {
        // Encontramos solapamientos, verificar si son relevantes
        for (const FOverlapResult& Overlap : Overlaps)
        {
            // Ignorar actores que no tienen colisión (podrían ser efectos visuales, etc.)
            if (Overlap.GetActor() && Overlap.GetActor()->GetActorEnableCollision())
            {
                // Si encontramos algún actor con colisión, la posición no es válida
                return false;
            }
        }
    }
    
    // 2. Comprobar si el caldero está alineado con el suelo
    // Hacer un trazado hacia abajo para ver si hay suelo cerca
    FHitResult HitResult;
    FVector TraceStart = PreviewLocation;
    FVector TraceEnd = TraceStart - FVector(0, 0, MaxGroundAlignmentHeight * 2.0f);
    
    bool bHitGround = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        TraceStart,
        TraceEnd,
        ECC_Visibility,
        QueryParams
    );
    
    if (!bHitGround)
    {
        // No encontramos suelo debajo del caldero
        return false;
    }
    
    // Comprobar la distancia al suelo
    float DistanceToGround = (HitResult.Location - TraceStart).Size();
    if (DistanceToGround > MaxGroundAlignmentHeight)
    {
        // El caldero está demasiado lejos del suelo
        return false;
    }
    
    // La posición es válida
    return true;
} 
// ----------------------------------- BROADCAST HELPER FUNCTIONS ---------------------------------------------- //
void ACauldronAltar::BroadcastBaseIngredientDropped() const
{
    OnBaseIngredientDropped.Broadcast(BaseIngredient);
}

UWitchPTInventoryItemInstance* ACauldronAltar::GetBaseIngredient() const
{
    return BaseIngredient;
}


