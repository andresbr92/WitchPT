
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
	/**
	 * @brief Singleton accessor to all native GameplayTags.
	 *
	 * @return Reference to the singleton instance of WitchPTGameplayTags.
	 * @note To use the Singleton:
	 * const FAuraGameplayTags& AuraGameplayTags = FAuraGameplayTags::Get();
	 */
struct FWitchPTGameplayTags
{
public:
	static FWitchPTGameplayTags& Get() { return GameplayTags; }
	static void InitializeGameplayTags();


	/**
	* Attributes Primary Tags
	*/
	FGameplayTag Attributes_Primary_Health;
	FGameplayTag Attributes_Primary_Age;
	FGameplayTag Attributes_Secondary_Mana;
	FGameplayTag Attributes_Secondary_Stamina;
	// State Tags
	FGameplayTag Character_State_Movement_Sprinting;
	FGameplayTag Character_State_Movement_Crouching;
	FGameplayTag Character_State_Movement_MovingToPosition;
	FGameplayTag Character_State_Dead;
	FGameplayTag Character_State_Aim;
	FGameplayTag Character_State_Blocked;
	
	FGameplayTag Character_State_Cauldron_InPosition;
	FGameplayTag Character_State_Cauldron_Carrying;
	FGameplayTag Character_State_Cauldron_Placing;
	FGameplayTag Character_State_Cauldron_InGame;
	FGameplayTag Character_State_Cauldron_BrewingPotion;
	
	FGameplayTag Character_State_Ritual_InPosition;
	FGameplayTag Character_State_Ritual_InGame;

	// Ability Tags
	FGameplayTag Ability_Sprint;
	FGameplayTag Ability_Jump;
	FGameplayTag Ability_Crouch;
	FGameplayTag Ability_Ritual_GoToPosition;
	FGameplayTag Ability_Ritual_ParticipatingInAltar;
	FGameplayTag Ability_Cauldron_ParticipatingInCauldron;
	FGameplayTag Ability_Cauldron_CarryingCauldron;
	FGameplayTag Ability_Cauldron_LookingForPlaceCauldron;
	FGameplayTag Ability_Cauldron_SetInPlace;
	FGameplayTag Ability_Cauldron_CancelSetInPlace;

	
	



	// Input Tags
	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;
	FGameplayTag InputTag_5;
	FGameplayTag InputTag_6;
	FGameplayTag InputTag_7;
	FGameplayTag InputTag_8;
	FGameplayTag InputTag_9;
	FGameplayTag InputTag_0;
	FGameplayTag InputTag_Ctrl;
	FGameplayTag InputTag_Alt;
	FGameplayTag InputTag_Shift;


	// --- Ritual Position Tags ---
	FGameplayTag Ritual_Position_1;
	FGameplayTag Ritual_Position_2;
	FGameplayTag Ritual_Position_3;
	FGameplayTag Ritual_Position_4;

	FGameplayTag Ritual_Input_Up;
	FGameplayTag Ritual_Input_Down;
	FGameplayTag Ritual_Input_Left;
	FGameplayTag Ritual_Input_Right;

	// --- Ritual Event Tags ---
	FGameplayTag Event_Ritual_InputSuccess;
	FGameplayTag Event_Ritual_InputFailure;
	FGameplayTag Event_Ritual_SequenceSuccess;
	FGameplayTag Event_Ritual_SequenceFailure;
	FGameplayTag Event_Ritual_CatastrophicFailure;
	
	

	// --- Event Tags ---
	FGameplayTag Event_Ritual_InputReceived; // Generic event when altar receives an input GE
	FGameplayTag Event_Ritual_TurnAdvanced;
	FGameplayTag Event_Ritual_SequenceFail;
	FGameplayTag Event_Ritual_CatastrophicFail;

	// Cauldron Tags
	FGameplayTag Cauldron_State_Empty;
	FGameplayTag Cauldron_State_Full;
	FGameplayTag Cauldron_State_PotionReady;




private:
	static FWitchPTGameplayTags GameplayTags;
	
	
};
