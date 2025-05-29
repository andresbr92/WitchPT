// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/BaseInteractableAltar.h"
#include "GameplayTagContainer.h" // Include for FGameplayTag
#include "MechanicsInterface.h"
#include "AbilitySystem/Interaction/IInteractableTarget.h"
#include "CauldronAltar.generated.h"

class UWitchPTInventoryItemFragment_IngredientDetails;
class UWitchPTInventoryItemFragment;
class UCauldronUserWidget;
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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBaseIngredientSetSignature, UWitchPTInventoryItemInstance*, IngredientInstance);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBaseIngredientIconSetSignature, TSubclassOf<UUserWidget>, BaseIngredientIcon);

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
    
    // Overrides
    virtual void BeginPlay() override;
    virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
    virtual void GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& OptionBuilder) override;
    virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
    virtual void UnoccupyPosition(ACharacter* Character, ABaseInteractionPosition* Position) override;
    
    // Core Properties
    UPROPERTY(EditAnywhere)
    FInteractionOption Option;
    
    UPROPERTY(ReplicatedUsing = OnRep_CauldronPhysicState, BlueprintReadWrite, VisibleAnywhere, Category = "Cauldron")
    TEnumAsByte<ECauldronPhysicState> CauldronPhysicState;

    // Base ingredient
    UPROPERTY(VisibleAnywhere)
    UWitchPTInventoryItemInstance* BaseIngredient;
    UPROPERTY(VisibleAnywhere)
    UWitchPTInventoryItemInstance* PrincipalIngredient;
    UPROPERTY(VisibleAnywhere)
    UWitchPTInventoryItemInstance* PotentiatorIngredient;
    
    UPROPERTY(ReplicatedUsing = OnRep_BaseIngredientIcon,Category= "Cauldron|Ingredients", VisibleAnywhere)
    TSubclassOf<UUserWidget> BaseIngredientIcon;

    UPROPERTY(ReplicatedUsing = OnRep_PrincipalIngredientIcon,Category= "Cauldron|Ingredients", VisibleAnywhere)
    TSubclassOf<UUserWidget> PrincipalIngredientIcon;

    UPROPERTY(ReplicatedUsing = OnRep_PotentiatorIngredientIcon,Category= "Cauldron|Ingredients", VisibleAnywhere)
    TSubclassOf<UUserWidget> PotentiatorIngredientIcon;
    
    // ----------------------------------- DELEGATES  ---------------------------------------------- //

    FOnBaseIngredientSetSignature OnBaseIngredientSetDelegate;
    FOnBaseIngredientIconSetSignature OnBaseIngredientIconSetDelegate;
    UPROPERTY(BlueprintAssignable, Category = "Cauldron|Placement")
    FOnECauldronPhysicStateChanged OnECauldronPhysicStateChanged;
    
    UPROPERTY(BlueprintAssignable, Category = "Cauldron|Placement")
    FOnCharacterPositioned OnCharacterPositioned;
    
    // UI Properties
    UPROPERTY(EditDefaultsOnly, Category= "Cauldron|UI")
    TSubclassOf<UCauldronUserWidget> CauldronUserWidgetClass;
    
    // Placement Properties
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Cauldron|Placement")
    UMaterialInterface* ValidPlacementMaterial;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Cauldron|Placement")
    UMaterialInterface* InvalidPlacementMaterial;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Cauldron|Placement")
    float PlacementCollisionCheckRadius = 50.0f;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Cauldron|Placement")
    float MaxGroundAlignmentHeight = 20.0f;
    
    // ----------------------------------- BREWING FUNCTIONS ---------------------------------------------- //
    
    void StartBrewingPotion(ACharacter* InteractingCharacter);
   
    void SetBaseIngredient(const ACharacter* RequestingCharacter, const TSubclassOf<UWitchPTInventoryItemDefinition>& IngredientItemDef);
    
    // ----------------------------------- ON REP FUNCTIONS ---------------------------------------------- //
    
    UFUNCTION()
    void OnRep_CauldronPhysicState();
    UFUNCTION()
    void OnRep_BaseIngredientIcon();
    UFUNCTION()
    void OnRep_PrincipalIngredientIcon();
    UFUNCTION()
    void OnRep_PotentiatorIngredientIcon();
    
    UFUNCTION()
    void PositionCharacterForBrewing(ACharacter* Character);
    
    UFUNCTION(BlueprintCallable, Category = "Cauldron|Brewing")
    ABaseInteractionPosition* GetAvailableBrewingPosition(ACharacter* Character);
    
    
    // ----------------------------------- CAULDRON MOVEMENT FUNCTIONS ---------------------------------------------- //
    void StartCarryCauldron(ACharacter* InteractingCharacter);
    
    UFUNCTION()
    void AttachToCharacter(ACharacter* Character);
    
    UFUNCTION()
    void DetachFromCharacter(ACharacter* Character);
    
    // ----------------------------------- PLACEMENT FUNCTIONS ---------------------------------------------- //
    void StartPlacementPreview(ACharacter* Character);
    void UpdatePlacementPreview(const FVector& HitLocation, const FVector& HitNormal);
    void FinalizePlacement();
    void CancelPlacement();
    void ApplyPlacementPreviewMaterial();
    
    UFUNCTION(Client, Unreliable)
    void Client_UpdatePlacementPreview(const FVector& HitLocation, const FVector& HitNormal);
    
    UFUNCTION(NetMulticast, Reliable)
    void Multicast_FinalizePlacement();
    
    // ----------------------------------- STATE QUERY FUNCTIONS ---------------------------------------------- //
    UFUNCTION(BlueprintPure, Category = "Cauldron|State")
    bool CanBePickedUp() const;
    
    UFUNCTION(BlueprintPure, Category = "Cauldron|State")
    bool IsBeingCarried() const;
    
    UFUNCTION(BlueprintPure, Category = "Cauldron|State")
    ACharacter* GetCarryingCharacter() const;
    
    UFUNCTION(BlueprintPure, Category = "Cauldron|Placement")
    ECauldronPlacementState GetPlacementState() const;
    
    UFUNCTION(BlueprintPure, Category = "Cauldron|Placement")
    bool IsInPlacementPreview() const;
    // ----------------------------------- GETTERS ---------------------------------------------- //
    
    UWitchPTInventoryItemInstance* GetBaseIngredient() const;
    
protected:
    // ----------------------------------- BROADCAST HELPER FUNCTIONS ---------------------------------------------- //
	// These functions handle event broadcasting and are called both from server-side code and OnRep functions
    void BroadcastBaseIngredientDropped() const;
    void BroadcastBaseIngredientIconSet() const;

    
private:
    UPROPERTY(Replicated)
    TObjectPtr<ACharacter> CarryingCharacter;
    
    UPROPERTY(EditDefaultsOnly, Category = "Cauldron|Movement")
    FName BackAttachSocketName = "BackpackSocket";
    
    UPROPERTY(EditDefaultsOnly, Category = "Cauldron|Movement")
    FVector DetachmentOffset = FVector(100.0f, 0.0f, 0.0f);
    
    // Placement Properties
    UPROPERTY(Replicated)
    ECauldronPlacementState CurrentPlacementState;
    
    UPROPERTY()
    TArray<UMaterialInterface*> OriginalMaterials;
    
    FVector PreviewLocation;
    FRotator PreviewRotation;
    
    // Private Utility Functions
    void RestoreOriginalMaterials();
    bool IsPlacementValid() const;
}; 