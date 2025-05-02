// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Item.h"
#include "AbilitySystem/Interaction/InteractionOption.h"
#include "GameplayTagContainer.h" // Added for FGameplayTag
#include "RitualInterface.h"
#include "RitualPosition.generated.h"

// Forward Declarations
class ARitualAltar;
class UAbilitySystemComponent;
struct FGameplayAbilitySpec;
class AController;
class UGameplayAbility;
class UBoxComponent;
class ACharacter;

UCLASS()
class WITCHPT_API ARitualPosition : public AItem, public IRitualInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARitualPosition();

	//~ Begin AActor Interface
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override; // Added BeginPlay
	//~ End AActor Interface

	//~ Begin IInteraction Interface (from AItem presumably)
	virtual void GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& OptionBuilder) override;
	//~ End IInteraction Interface

	//~ Getters
	UFUNCTION(BlueprintPure, Category = "Ritual")
	bool IsOccupied() const { return bIsOccupied; }

	UFUNCTION(BlueprintPure, Category = "Ritual")
	ACharacter* GetOccupyingCharacter() const { return OccupyingCharacter; }

	UFUNCTION(BlueprintPure, Category = "Ritual")
	ARitualAltar* GetRitualAltar() const { return RitualAltar; }

	UFUNCTION(BlueprintPure, Category = "Ritual")
	FGameplayTag GetPositionTag() const { return PositionTag; }
	//~ End Getters

	//~ Setters (Called by Altar or GA) - Ensure proper authority checks if needed
	// These should likely only be called on the server
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void SetOccupied(ACharacter* Character);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void SetUnoccupied();


	UFUNCTION(BlueprintCallable)
	void SetPositionTag(const FGameplayTag& NewTag);
	UFUNCTION(BlueprintCallable)
	void SetRitualAltar(ARitualAltar* Altar) { RitualAltar = Altar; }


	/**
	 * Interface functions
	 */
	virtual void HandleStartRitualRequest_Implementation(ACharacter* RequestingCharacter) override;
	virtual void HandlePlayerInput_Implementation(ACharacter* InputCharacter, const FGameplayTag& InputTag) override;
protected:
	

	// Reference to the parent altar, should be set on spawn or in BeginPlay
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ritual", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ARitualAltar> RitualAltar;

	// Unique tag identifying this position (e.g., Ritual.Position.1). Set per instance.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Ritual")
	FGameplayTag PositionTag;

	// Is this position currently occupied by a player? Replicated.
	UPROPERTY(ReplicatedUsing = OnRep_IsOccupied, BlueprintReadOnly, Category = "Ritual|State")
	bool bIsOccupied = false;

	// Which character is currently occupying this position? Replicated.
	UPROPERTY(ReplicatedUsing = OnRep_OccupyingCharacter, BlueprintReadOnly, Category = "Ritual|State")
	TObjectPtr<ACharacter> OccupyingCharacter = nullptr;


	// Variables potentially used by the base AItem interaction system
	UPROPERTY()
	TWeakObjectPtr<UAbilitySystemComponent> InteractingASC;

	UPROPERTY()
	TWeakObjectPtr<AController> InteractingController;

	

	//~ Replication Notifies
	UFUNCTION()
	void OnRep_IsOccupied();

	UFUNCTION()
	void OnRep_OccupyingCharacter();
	//~ End Replication Notifies

	// Interaction Logic Helper (Called from GatherInteractionOptions or a specific interaction GA)
	void HandleInteraction(ACharacter* InteractingCharacter);
private:
	void SetCharacterToPosition(ACharacter* Character);
	void RemoveCharacterFromPosition();

};
