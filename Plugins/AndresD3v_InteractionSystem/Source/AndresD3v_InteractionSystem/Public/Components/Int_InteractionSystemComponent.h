// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/WeakObjectPtrTemplates.h"
#include "SmartObjectSubsystem.h"
#include "Int_InteractionStructLibrary.h"
#include "Components/ActorComponent.h"
#include "Runtime/Engine/Classes/Components/ActorComponent.h"
#include "Int_InteractionSystemComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), ClassGroup=(AndresD3v), meta=(BlueprintSpawnableComponent))
class ANDRESD3V_INTERACTIONSYSTEM_API UInt_InteractionSystemComponent : public UActorComponent
{
		GENERATED_BODY()

public:

	UInt_InteractionSystemComponent();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	/**
	 * Retrieves the interaction system component from an actor
	 * @param Actor The actor to query.
	 * @return The interaction system component.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="AndresD3v|InteractionSystem", meta=(DefaultToSelf))
	static UInt_InteractionSystemComponent* GetInteractionSystemComponent(const AActor* Actor);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category="AndresD3v|InteractionSystem")
	void Server_CycleInteractableActors(bool bNext);
protected:
	// ================================= ONREP FUNCTIONS
	UFUNCTION()
	virtual void OnInteractableActorChanged(AActor* OldActor);

	UFUNCTION()
	virtual void OnInteractableActorsNumChanged();
	UFUNCTION()
	void OnInteractingOptionChanged(int32 PrevOptionIndex);
	// ================================= PROPERTIES
	/**
	 * Array of potential interactable actors.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AndresD3v|InteractionSystem")
	TArray<TObjectPtr<AActor>> InteractableActors;
	
	/**
	 * Number of potential interactable actors, replicated to the owning client.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing=OnInteractableActorsNumChanged, Category="AndresD3v|InteractionSystem")
	int32 NumOfInteractableActors{0};

	/**
	 * Default filter for searching interactable actors.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AndresD3v|InteractionSystem")
	FSmartObjectRequestFilter DefaultRequestFilter;

	/**
	 * Current selected interactable actor, replicated for the owner only.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AndresD3v|InteractionSystem", ReplicatedUsing=OnInteractableActorChanged)
	TObjectPtr<AActor> CurrentInteractableActor;
	
	/**
	 * If checked, whenever potential interactable actors changes, the fist actor in the list will be selected as currentInteractableActor.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing=OnInteractableActorsNumChanged, Category="AndresD3v|InteractionSystem")
	bool bNewActorHasPriority{false};
	
	/**
	 * Current available interaction options, replicated for the owner only.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AndresD3v|InteractionSystem")
	TArray<FInt_InteractionOption> InteractionOptions;

	/**
	 * Indicates if an interaction is in progress.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AndresD3v|InteractionSystem")
	bool bIsInteracting{false};

	/**
	 * Current interacting option index (-1 if no interaction).
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AndresD3v|InteractionSystem", ReplicatedUsing=OnInteractingOptionChanged)
	int32 InteractingOption{INDEX_NONE};

	/**
	 * Map of smart object slog handles to delegate handles.
	 */
	TMap<FSmartObjectSlotHandle, FDelegateHandle> SlotCallbacks;
};
