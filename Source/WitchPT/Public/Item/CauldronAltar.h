// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/BaseInteractableAltar.h"
#include "GameplayTagContainer.h" // Include for FGameplayTag
#include "MechanicsInterface.h"
#include "AbilitySystem/Interaction/IInteractableTarget.h"
#include "CauldronAltar.generated.h"

class ACauldronPosition;
class ACharacter;

// Estructura para contener secuencias de ingredientes
USTRUCT(BlueprintType)
struct FIngredientSequence
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    TArray<FGameplayTag> Ingredients;
};

// Enum for cauldron inputs
UENUM(BlueprintType)
enum class ECauldronInput : uint8
{
    Ingredient1 UMETA(DisplayName = "Ingredient 1"),
    Ingredient2 UMETA(DisplayName = "Ingredient 2"),
    Ingredient3 UMETA(DisplayName = "Ingredient 3"),
    Ingredient4 UMETA(DisplayName = "Ingredient 4"),
    Ingredient5 UMETA(DisplayName = "Ingredient 5"),
    None        UMETA(DisplayName = "None")
};

UENUM(BlueprintType)
enum ECauldronPhysicState
{
    Static UMETA(DisplayName = "InPlace"),
    Moving UMETA(DisplayName = "Moving"),
    Previewing UMETA(DisplayName = "Previewing")
};

// Enum para el estado de validación de la posición del caldero
UENUM(BlueprintType)
enum class ECauldronPlacementState : uint8
{
    Valid UMETA(DisplayName = "Valid"),
    Invalid UMETA(DisplayName = "Invalid"),
    None UMETA(DisplayName = "None")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnECauldronPhysicStateChanged, ECauldronPhysicState, PhysicState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterPositioned, bool, bWasSuccessful);
/**
 * Cauldron altar allows players to add ingredients in any order (unlike ritual's sequential inputs)
 */
UCLASS()
class WITCHPT_API ACauldronAltar : public ABaseInteractableAltar, public IInteractableTarget, public IMechanicsInterface
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ACauldronAltar();
    virtual void BeginPlay() override;
    virtual void GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& OptionBuilder) override;
    UPROPERTY(EditAnywhere)
    FInteractionOption Option;
    
    virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
    
    UPROPERTY(ReplicatedUsing = OnRep_CauldronPhysicState, BlueprintReadWrite, VisibleAnywhere, Category = "Cauldron")
    TEnumAsByte<ECauldronPhysicState> CauldronPhysicState;

    // --- Modified functions for interaction without RPC ---

    

    
    // Funciones normales que reemplazan a los RPCs
    void StartBrewingPotion(ACharacter* InteractingCharacter);
    void StartCarryCauldron(ACharacter* InteractingCharacter);
    void StartPlacementPreview(ACharacter* Character);
    void UpdatePlacementPreview(const FVector& HitLocation, const FVector& HitNormal);
    void FinalizePlacement();
    void CancelPlacement();

    UFUNCTION(Client, Reliable)
    void Client_OnCharacterPositioned();
    
    UFUNCTION()
    void AttachToCharacter(ACharacter* Character);
    
    UFUNCTION()
    void DetachFromCharacter(ACharacter* Character);
    
    UFUNCTION()
    void PositionCharacterForBrewing(ACharacter* Character);
    
    UFUNCTION(BlueprintPure, Category = "Cauldron|State")
    bool CanBePickedUp() const;
    
    UFUNCTION(BlueprintPure, Category = "Cauldron|State")
    bool IsBeingCarried() const;
    
    UFUNCTION(BlueprintPure, Category = "Cauldron|State")
    ACharacter* GetCarryingCharacter() const;
    
    UFUNCTION(BlueprintCallable, Category = "Cauldron|Brewing")
    ABaseInteractionPosition* GetAvailableBrewingPosition(ACharacter* Character);

    UFUNCTION()
    void OnRep_CauldronPhysicState();
    
    // -- For placement preview functions --
    //Delegate
    UPROPERTY(BlueprintAssignable, Category = "Cauldron|Placement")
    FOnECauldronPhysicStateChanged OnECauldronPhysicStateChanged;
    UPROPERTY(BlueprintAssignable, Category = "Cauldron|Placement")
    FOnCharacterPositioned OnCharacterPositioned;
    
    UFUNCTION(BlueprintPure, Category = "Cauldron|Placement")
    ECauldronPlacementState GetPlacementState() const;
   
    UFUNCTION(BlueprintPure, Category = "Cauldron|Placement")
    bool IsInPlacementPreview() const;
    
    // Material a aplicar durante la previsualización
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Cauldron|Placement")
    UMaterialInterface* ValidPlacementMaterial;
    
    // Material a aplicar cuando la posición no es válida
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Cauldron|Placement")
    UMaterialInterface* InvalidPlacementMaterial;
    
    // Radio de detección de colisiones para validar la posición
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Cauldron|Placement")
    float PlacementCollisionCheckRadius = 50.0f;
    
    // Altura máxima permitida para alineación con el suelo
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Cauldron|Placement")
    float MaxGroundAlignmentHeight = 20.0f;
    void ApplyPlacementPreviewMaterial();
    
private:
    // Character currently carrying the cauldron
    UPROPERTY(Replicated)
    TObjectPtr<ACharacter> CarryingCharacter;
    
    // Socket name for attaching the cauldron to the character
    UPROPERTY(EditDefaultsOnly, Category = "Cauldron|Movement")
    FName BackAttachSocketName = "BackpackSocket";
    
    // Offset for placing the cauldron when detached
    UPROPERTY(EditDefaultsOnly, Category = "Cauldron|Movement")
    FVector DetachmentOffset = FVector(100.0f, 0.0f, 0.0f);
    
    // Estado de colocación actual
    UPROPERTY(Replicated)
    ECauldronPlacementState CurrentPlacementState;
    
    // Materiales originales para restaurar después de la previsualización
    UPROPERTY()
    TArray<UMaterialInterface*> OriginalMaterials;
    
    // Aplica el material de previsualización según el estado de colocación
    
    // Restaura los materiales originales después de salir del modo previsualización
    void RestoreOriginalMaterials();
    
    // Verifica si la posición actual es válida
    bool IsPlacementValid() const;
    
    // Posición guardada para la previsualización
    FVector PreviewLocation;
    
    // Rotación guardada para la previsualización
    FRotator PreviewRotation;
    
    // RPCs ahora son funciones internas para responder a llamadas del MechanicComponent
    UFUNCTION(NetMulticast, Reliable)
    void Multicast_FinalizePlacement();
    
    UFUNCTION(Client, Unreliable)
    void Client_UpdatePlacementPreview(const FVector& HitLocation, const FVector& HitNormal);
}; 