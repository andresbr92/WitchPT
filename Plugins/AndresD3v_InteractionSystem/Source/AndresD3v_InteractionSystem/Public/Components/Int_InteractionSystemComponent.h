// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/WeakObjectPtrTemplates.h"
#include "SmartObjectSubsystem.h"
#include "Int_InteractionStructLibrary.h"
#include "Components/ActorComponent.h"
#include "Runtime/Engine/Classes/Components/ActorComponent.h"
#include "Int_InteractionSystemComponent.generated.h"
/**
 * Delegate for interaction events.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInteractionEventSignature);
/**
 * Delegate for changes in the interactable actor.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInteractableActorChangedSignature, AActor*, OldActor, AActor*, NewActor);

/**
 * Delegate for changes in the interacting state.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractingStateChangedSignature, bool, bInteracting);

/**
 * Delegate for changes in the number of interactable actors.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractableActorNumChangedSignature, int32, ActorsNum);

/**
 * Component for managing interactions with smart objects.
 */

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), ClassGroup=(AndresD3v), meta=(BlueprintSpawnableComponent))
class ANDRESD3V_INTERACTIONSYSTEM_API UInt_InteractionSystemComponent : public UActorComponent
{
		GENERATED_BODY()

public:

	UInt_InteractionSystemComponent();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	// ================================= FUNCTIONS
	
	/**
	 * Retrieves the interaction system component from an actor.
	 * @param Actor The actor to query.
	 * @return The interaction system component.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="AndresD3v|InteractionSystem", meta=(DefaultToSelf))
	static UInt_InteractionSystemComponent* GetInteractionSystemComponent(const AActor* Actor);
	
	/**
	 * Cycles through interactable actors.
	 * @param bNext Whether to cycle to the next actor.
	 */
	UFUNCTION(Server, Reliable, BlueprintCallable, Category="AndresD3v|InteractionSystem")
	void Server_CycleInteractableActors(bool bNext);

	/**
	 * Triggers a search for potential interactable actors.
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="AndresD3v|InteractionSystem")
	void SearchInteractableActors();
	
	/**
	 * Sets a new array of interactable actors.
	 * @param NewActors The new interactable actors array.
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="AndresD3v|InteractionSystem")
	void SetInteractableActors(TArray<AActor*> NewActors);
	
	/**
	 * Sets the number of interactable actors.
	 * @param NewNum The new number of interactable actors.
	 */
	void SetInteractableActorsNum(int32 NewNum);
	
	/**
	 * Retrieves the array of interactable actors.
	 * @return The interactable actors.
	 */
	TArray<AActor*> GetInteractableActors() const { return InteractableActors; }
	
	/**
	 * Retrieves the number of interactable actors.
	 * @return The number of interactable actors. 可交互演员数量。
	 */
	int32 GetNumOfInteractableActors() const { return NumOfInteractableActors; }

	/**
	 * Sets the current interactable actor.
	 * @param InActor The actor to set. 要设置的演员。
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="AndresD3v|InteractionSystem")
	void SetCurrentInteractableActor(AActor* InActor);
	
	/**
	 * Retrieves the number of interactable actors.
	 * @return The number of interactable actors. 可交互演员数量。
	 */
	AActor* GetInteractableActor() const { return CurrentInteractableActor; }

	/**
	 * Delegate for when the interactable actor changes.
	 */
	UPROPERTY(BlueprintAssignable)
	FInteractableActorChangedSignature OnInteractableActorChangedEvent;

	/**
	 * Delegate for when the number of interactable actors changes.
	 */
	UPROPERTY(BlueprintAssignable)
	FInteractableActorNumChangedSignature OnInteractableActorNumChangedEvent;

	/**
	 * Delegate for when the interacting state changes.
	 */
	UPROPERTY(BlueprintAssignable)
	FInteractingStateChangedSignature OnInteractingStateChangedEvent;

	/**
	 * Delegate for when the interaction options change.
	 */
	UPROPERTY(BlueprintAssignable)
	FInteractionEventSignature OnInteractionOptionsChangedEvent;

	/**
	 * Delegate for when a search for interactable actors is triggered.
	 */
	UPROPERTY(BlueprintAssignable)
	FInteractionEventSignature OnSearchInteractableActorsEvent;
	
	/**
	 * Retrieves de smart objet request filter.
	 * @return The smart object request filter.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, BlueprintNativeEvent, Category="AndresD3v|InteractionSystem")
	FSmartObjectRequestFilter GetSmartObjectRequestFilter();
	virtual FSmartObjectRequestFilter GetSmartObjectRequestFilter_Implementation();
	
	/**
	 * Starts an interaction with the specified option index.
	 * @param NewIndex The interaction option index.
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="AndresD3v|InteractionSystem")
	virtual void StartInteraction(int32 NewIndex = 0);
	
	/**
	 * Ends the current interaction.
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="AndresD3v|InteractionSystem")
	virtual void EndInteraction();

	/**
	 * Performs an instant interaction with the specified option index.
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="AndresD3v|InteractionSystem")
	void InstantInteraction(int32 NewIndex = 0);

	/**
	 * Checks if an interaction is in progress.
	 * @return True if interacting, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="GGS|InteractionSystem")
	bool IsInteracting() const;
	/**
	 * Retrieves the current interacting option index.
	 * @return The interacting option index.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="GGS|InteractionSystem")
	int32 GetInteractingOption() const;
	
	/**
	 * Retrieves the current interaction options.
	 * @return The interaction options.
	 */
	const TArray<FInt_InteractionOption>& GetInteractionOptions() const { return InteractionOptions; }


	
protected:

	/**
	 * Called when the interactable actor changes.
	 * @param OldActor The previous interactable actor.
	 */
	UFUNCTION()
	virtual void OnInteractableActorChanged(AActor* OldActor);

	/**
	 * Called when the number of interactable actors changes.
	 */
	UFUNCTION()
	virtual void OnInteractableActorsNumChanged();
	
	/**
	 * Called when the potential interactable actors change.
	 */
	UFUNCTION(BlueprintNativeEvent, Category="AndresD3v|InteractionSystem")
	void OnInteractableActorsChanged();
	/**
	 * Called when a smart object event occurs.
	 * @param EventData The smart object event data.
	 */
	UFUNCTION()
	virtual void OnSmartObjectEventCallback(const FSmartObjectEventData& EventData);

	/**
	 * Called when interaction options change.
	 */
	UFUNCTION()
	virtual void OnInteractionOptionsChanged();
	/**
	 * Called when the interacting option index changes.
	 * @param PrevOptionIndex The previous option index.
	 */
	UFUNCTION(BlueprintNativeEvent, Category="AndresD3v|InteractionSystem")
	void OnInteractingOptionChanged(int32 PrevOptionIndex);

	
	virtual void RefreshOptionsForActor();

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
	 * Current selected interactable actor, replicated for the owner only.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AndresD3v|InteractionSystem", ReplicatedUsing=OnInteractableActorChanged)
	TObjectPtr<AActor> CurrentInteractableActor;
	
	/**
	 * Default filter for searching interactable actors.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AndresD3v|InteractionSystem")
	FSmartObjectRequestFilter DefaultRequestFilter;
	
	/**
	 * If checked, whenever potential interactable actors changes, the fist actor in the list will be selected as currentInteractableActor.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing=OnInteractableActorsNumChanged, Category="AndresD3v|InteractionSystem")
	bool bNewActorHasPriority{false};
	
	/**
	 * Current available interaction options, replicated for the owner only.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AndresD3v|InteractionSystem", ReplicatedUsing=OnInteractionOptionsChanged)
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
