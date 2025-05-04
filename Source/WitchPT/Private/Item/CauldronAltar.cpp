#include "Item/CauldronAltar.h"
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
        ApplyPlacementPreviewMaterial();
    }
    else
    {
        // Cauldron is stationary - update visuals if needed
        SetActorEnableCollision(true);
        RestoreOriginalMaterials();
    }
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
    
    // If the cauldron is being carried by this character, detach it
    if (IsBeingCarried() && CarryingCharacter == InteractingCharacter)
    {
        StartPlacementPreview(InteractingCharacter);
        return;
    }
    
    // Otherwise, try to position the character for brewing
    bool bSuccess = PositionCharacterForBrewing(InteractingCharacter);
    if (!bSuccess)
    {
        UE_LOG(LogTemp, Warning, TEXT("ACauldronAltar::StartBrewingPotion: Failed to position character for brewing"));
    }
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
    DestroyBrewingPositions();
    
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
    else
    {
        // If no socket exists, just attach to the character's root
        AttachToComponent(Character->GetRootComponent(), AttachRules);
        UE_LOG(LogTemp, Warning, TEXT("ACauldronAltar::AttachToCharacter: Socket %s not found, attached to root"), 
               *BackAttachSocketName.ToString());
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
    CreateBrewingPositions();
    
    UE_LOG(LogTemp, Log, TEXT("ACauldronAltar::DetachFromCharacter: Cauldron detached from %s"), *Character->GetName());
    
    // Play placement sound or effects
    // PlayPlacementEffects();
}

bool ACauldronAltar::PositionCharacterForBrewing(ACharacter* Character)
{
    if (!Character)
    {
        UE_LOG(LogTemp, Warning, TEXT("ACauldronAltar::PositionCharacterForBrewing: Invalid character"));
        return false;
    }
    
    // Obtener una posición disponible
    ACauldronPosition* BrewingPosition = GetAvailableBrewingPosition(Character);
    
    if (BrewingPosition)
    {
        // Posicionar al personaje en esta posición
        // return BrewingPosition->
        return false;
    }
    else
    {
        // Fallback: si no hay posiciones disponibles, colocar al personaje en frente del caldero
        UE_LOG(LogTemp, Log, TEXT("ACauldronAltar::PositionCharacterForBrewing: Using fallback positioning"));
        
        // Si no hay posiciones, crear una posición por defecto frente al caldero
        FVector CharacterLocation = GetActorLocation() + GetActorForwardVector() * 150.0f;
        FRotator CharacterRotation = (GetActorLocation() - CharacterLocation).Rotation();
        
        // Teleportar al personaje a esta posición
        Character->SetActorLocationAndRotation(CharacterLocation, CharacterRotation, false, nullptr, ETeleportType::TeleportPhysics);
        return true;
    }
}

// Implementación de las nuevas funciones para las posiciones de elaboración
void ACauldronAltar::CreateBrewingPositions()
{
    // Only create brewing positions on the server
    if (!HasAuthority() || !CauldronPositionClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("ACauldronAltar::CreateBrewingPositions: Missing authority or position class"));
        return;
    }
    
    // Make sure we have no previous positions
    DestroyBrewingPositions();
    
    UE_LOG(LogTemp, Log, TEXT("ACauldronAltar::CreateBrewingPositions: Creating %d brewing positions"), BrewingPositionTransforms.Num());
    
    // Spawn a CauldronPosition for each transform
    for (const FTransform& PosTransform : BrewingPositionTransforms)
    {
        // Transform relative to world space
        FTransform WorldTransform = PosTransform * GetActorTransform();
        
        // Get location and rotation from the transform
        FVector Location = WorldTransform.GetLocation();
        FRotator Rotation = WorldTransform.Rotator();
        
        // Spawn parameters
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
        
        // Spawn the brewing position
        ACauldronPosition* NewPosition = GetWorld()->SpawnActor<ACauldronPosition>(
            CauldronPositionClass, 
            Location, 
            Rotation, 
            SpawnParams);
        
        if (NewPosition)
        {
            // Initialize the position
            // Note: Implement this method in CauldronPosition if needed
            // NewPosition->SetCauldronAltar(this);
            
            // Add to our array
            SpawnedBrewingPositions.Add(NewPosition);
            
            UE_LOG(LogTemp, Log, TEXT("ACauldronAltar::CreateBrewingPositions: Created position at %s"), *Location.ToString());
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("ACauldronAltar::CreateBrewingPositions: Failed to spawn position"));
        }
    }
}

void ACauldronAltar::DestroyBrewingPositions()
{
    // Only destroy positions on the server
    if (!HasAuthority())
    {
        return;
    }
    
    // Destroy all brewing positions
    for (ACauldronPosition* Position : SpawnedBrewingPositions)
    {
        if (Position)
        {
            Position->Destroy();
        }
    }
    
    // Clear the array
    SpawnedBrewingPositions.Empty();
    
    UE_LOG(LogTemp, Log, TEXT("ACauldronAltar::DestroyBrewingPositions: All brewing positions destroyed"));
}

ACauldronPosition* ACauldronAltar::GetAvailableBrewingPosition(ACharacter* Character)
{
    // Verificar que tengamos posiciones
    if (SpawnedBrewingPositions.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("ACauldronAltar::GetAvailableBrewingPosition: No brewing positions available"));
        return nullptr;
    }
    
    // Buscar la primera posición disponible
    for (ACauldronPosition* Position : SpawnedBrewingPositions)
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
    if (!Character || !HasAuthority() || Character != CarryingCharacter)
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
    SetActorScale3D(FVector(1.0f, 1.0f, 1.0f));
    
    UE_LOG(LogTemp, Log, TEXT("ACauldronAltar::StartPlacementPreview: Cauldron in placement preview mode"));
}

void ACauldronAltar::UpdatePlacementPreview(const FVector& HitLocation, const FVector& HitNormal)
{
    if (!IsInPlacementPreview() || !HasAuthority())
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
    
    // Establecer la nueva posición y rotación
    SetActorLocation(PreviewLocation);
    // SetActorRotation(PreviewRotation);
    
    // Verificar si la posición es válida
    CurrentPlacementState = IsPlacementValid() ? ECauldronPlacementState::Valid : ECauldronPlacementState::Invalid;
    
    // Actualizar el material según el estado
    // ApplyPlacementPreviewMaterial();
    
    // Debug
}

bool ACauldronAltar::FinalizePlacement()
{
    if (!IsInPlacementPreview() || !HasAuthority())
    {
        return false;
    }
    
    // // Comprobar si la posición es válida
    // if (CurrentPlacementState != ECauldronPlacementState::Valid)
    // {
    //     UE_LOG(LogTemp, Warning, TEXT("ACauldronAltar::FinalizePlacement: Cannot place cauldron in invalid position"));
    //     // Opcionalmente, reproducir sonido de error
    //     return false;
    // }
    
    // Establecer la posición final
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
    CauldronPhysicState = ECauldronPhysicState::Static;

    OnECauldronPhysicStateChanged.Broadcast(ECauldronPhysicState::Static);

    
    
    // Crear posiciones de elaboración alrededor del caldero
    CreateBrewingPositions();
    
    UE_LOG(LogTemp, Log, TEXT("ACauldronAltar::FinalizePlacement: Cauldron placed successfully"));
    
    return true;
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