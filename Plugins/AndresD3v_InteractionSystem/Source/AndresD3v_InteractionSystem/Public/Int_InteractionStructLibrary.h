#pragma once
#include "SmartObjectSubsystem.h"
#include "SmartObjectTypes.h"
#include "Engine/DataTable.h"
#include "UObject/Object.h"
#include "SmartObjectRequestTypes.h"
#include "Int_InteractionStructLibrary.generated.h"

class UInt_InteractionDefinition;
USTRUCT(DisplayName="Interaction Entrance")
struct FInt_SmartObjectInteractionEntranceData : public FSmartObjectDefinitionData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Interaction", meta=(DisplayName="Definition"))
	TSoftObjectPtr<UInt_InteractionDefinition> DefinitionDA{nullptr};
};
USTRUCT(BlueprintType)
struct ANDRESD3V_INTERACTIONSYSTEM_API FInt_InteractionOption
{
	GENERATED_BODY()
	
	/**
	 * Interaction definition associated with this option.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	TObjectPtr<UInt_InteractionDefinition> Definition{nullptr};

	/**
	 * Smart object request result for this option. Not replicated.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, NotReplicated, Category="Interaction")
	FSmartObjectRequestResult RequestResult;
	
	/**
	 * Smart object behavior definition for this option.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, NotReplicated, Category="Interaction")
	TObjectPtr<const USmartObjectBehaviorDefinition> BehaviorDefinition;
	/**
	 * Index of the associated smart object slot, used for UI sorting.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	int32 SlotIndex{-1};
	/**
	 * State of the associated smart object slot.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	ESmartObjectSlotState SlotState{ESmartObjectSlotState::Free};
	
	/**
	 * Equality operator for comparing interaction options.
	 */
	friend bool operator==(const FInt_InteractionOption& Lhs, const FInt_InteractionOption& RHS);

	/**
	 * Inequality operator for comparing interaction options.
	 */
	friend bool operator!=(const FInt_InteractionOption& Lhs, const FInt_InteractionOption& RHS);

	/**
	 * Less-than operator for sorting interaction options by slot index.
	 */
	friend bool operator<(const FInt_InteractionOption& Lhs, const FInt_InteractionOption& RHS);

	/**
	 * Converts the interaction option to a string representation.
	 * @return String representation of the option.
	 */
	FString ToString() const;
};


