
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
	FGameplayTag Ability_Interact;
	FGameplayTag Ability_Sprint;
	FGameplayTag Ability_Jump;
	FGameplayTag Ability_Crouch;
	FGameplayTag Ability_Ritual_GoToPosition;
	FGameplayTag Ability_Ritual_ParticipatingInRitual;
	FGameplayTag Ability_Ritual_CancelParticipatingInRitual;
	FGameplayTag Ability_Ritual_Start;
	FGameplayTag Ability_Ritual_SendInput;





	
	FGameplayTag Ability_Cauldron_ParticipatingInCauldron;
	FGameplayTag Ability_Cauldron_CancelParticipatingInCauldron;
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
	FGameplayTag InputTag_Tab;
	// UI Tags
	FGameplayTag UI_Layer_Game;
	FGameplayTag UI_Layer_GameMenu;
	FGameplayTag UI_Layer_Menu;
	FGameplayTag UI_Layer_Modal;


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
	FGameplayTag Event_Ritual_InputReceived;
	FGameplayTag Event_Ritual_TurnAdvanced;
	FGameplayTag Event_Ritual_SequenceFail;
	FGameplayTag Event_Ritual_CatastrophicFail;

	// Cauldron Tags
	FGameplayTag Cauldron_State_Empty;
	FGameplayTag Cauldron_State_Full;
	FGameplayTag Cauldron_State_PotionReady;
	FGameplayTag Event_Cauldron_CharacterPositioned;


	// Communication tags
	FGameplayTag Communication_Message;

	// --- Items Tags ---
	FGameplayTag Item;
	FGameplayTag Item_Consumable;
	FGameplayTag Item_Consumable_Potion;
	FGameplayTag Item_Consumable_Potion_Tier_0;
	FGameplayTag Item_Consumable_Potion_Tier_1;
	FGameplayTag Item_Consumable_Potion_Tier_2;
	FGameplayTag Item_Consumable_Potion_Tier_3;
	
	FGameplayTag Item_Consumable_Ingredient;
	FGameplayTag Item_Consumable_Ingredient_Tier_0;
	FGameplayTag Item_Consumable_Ingredient_Tier_1;
	FGameplayTag Item_Consumable_Ingredient_Tier_2;
	FGameplayTag Item_Consumable_Ingredient_Tier_3;


	FGameplayTag Item_Consumable_Ingredient_CanBeUsedIn_BaseSlot;
	FGameplayTag Item_Consumable_Ingredient_CanBeUsedIn_PrincipalSlot;
	FGameplayTag Item_Consumable_Ingredient_CanBeUsedIn_ModifierSlot;

	FGameplayTag Item_Consumable_Ingredient_Category_Base;
	FGameplayTag Item_Consumable_Ingredient_Category_Base_LiquidSolvent_Water;
	FGameplayTag Item_Consumable_Ingredient_Category_Base_LiquidSolvent_Oil;
	FGameplayTag Item_Consumable_Ingredient_Category_Base_LiquidSolvent_Blood;
	
	
	FGameplayTag Item_Consumable_Ingredient_Category_Principal;
	FGameplayTag Item_Consumable_Ingredient_Category_Principal_Herbal;
	FGameplayTag Item_Consumable_Ingredient_Category_Principal_Mineral;
	FGameplayTag Item_Consumable_Ingredient_Category_Principal_Animal;

	




	// --------------------------- Modifier Tags ---------------------------
	FGameplayTag Item_Consumable_Ingredient_Category_Modifier;
	// --------------------------- Modifier Potency Tags ---------------------------
	FGameplayTag Item_Consumable_Ingredient_Category_Modifier_Potency_Increase_Float;
	FGameplayTag Item_Consumable_Ingredient_Category_Modifier_Potency_Increase_Percentage;
	// --------------------------- Modifier Duration Tags ---------------------------
	FGameplayTag Item_Consumable_Ingredient_Category_Modifier_Duration_Increase_Float;
	FGameplayTag Item_Consumable_Ingredient_Category_Modifier_Duration_Increase_Percentage;
	// --------------------------- Modifier Charges Tags ---------------------------
	FGameplayTag Item_Consumable_Ingredient_Category_Modifier_Charges_Increase_Float;
	// --------------------------- Modifier Application Tags ---------------------------
	FGameplayTag Item_Consumable_Ingredient_Category_Modifier_Target_Self;
	FGameplayTag Item_Consumable_Ingredient_Category_Modifier_Target_Other;
	FGameplayTag Item_Consumable_Ingredient_Category_Modifier_Target_Area;
	




private:
	static FWitchPTGameplayTags GameplayTags;
	
	
};
