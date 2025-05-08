#include "FWitchPTGameplayTags.h"

#include "GameplayTagsManager.h"

FWitchPTGameplayTags FWitchPTGameplayTags::GameplayTags;

void FWitchPTGameplayTags::InitializeGameplayTags()
{
	GameplayTags.Attributes_Primary_Health = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Vital.Health"));
	GameplayTags.Attributes_Primary_Age = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Vital.Age"));
	GameplayTags.Attributes_Secondary_Mana = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Vital.Mana"));
	GameplayTags.Attributes_Secondary_Stamina = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Vital.Stamina"));
	GameplayTags.Character_State_Movement_Sprinting = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Character.State.Movement.Sprinting"));
	GameplayTags.Character_State_Movement_Crouching = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Character.State.Movement.Crouching"));
	GameplayTags.Character_State_Movement_MovingToPosition = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Character.State.Movement.MovingToPosition"));
	GameplayTags.Character_State_Dead = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Character.State.Dead"));
	GameplayTags.Character_State_Aim = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Character.State.Aim"));
	GameplayTags.Character_State_Blocked = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Character.State.Blocked"));
	GameplayTags.Character_State_Cauldron_InPosition = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Character.State.Cauldron.InPosition"));
	GameplayTags.Character_State_Cauldron_Carrying = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Character.State.Cauldron.Carrying"));
	GameplayTags.Character_State_Cauldron_Placing = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Character.State.Cauldron.Placing"));
	GameplayTags.Character_State_Cauldron_InGame = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Character.State.Cauldron.InGame"));
	GameplayTags.Character_State_Cauldron_BrewingPotion = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Character.State.Cauldron.BrewingPotion"));
	GameplayTags.Character_State_Ritual_InPosition = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Character.State.Ritual.InPosition"));
	GameplayTags.Character_State_Ritual_InGame = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Character.State.Ritual.InGame"));

	GameplayTags.Ability_Interact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Interact"));
	GameplayTags.Ability_Sprint = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Sprint"));
	GameplayTags.Ability_Jump = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Jump"));
	GameplayTags.Ability_Crouch = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Crouch"));
	GameplayTags.Ability_Ritual_GoToPosition = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Ritual.GoToPosition"));
	GameplayTags.Ability_Ritual_ParticipatingInAltar = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Ritual.ParticipatingInAltar"));
	// Cauldron Abilities
	GameplayTags.Ability_Cauldron_ParticipatingInCauldron = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Cauldron.ParticipatingInCauldron"));
	GameplayTags.Ability_Cauldron_CarryingCauldron = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Cauldron.CarryingCauldron"));
	GameplayTags.Ability_Cauldron_LookingForPlaceCauldron = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Cauldron.LookingForPlaceCauldron"));
	GameplayTags.Ability_Cauldron_SetInPlace = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Cauldron.SetInPlace"));
	GameplayTags.Ability_Cauldron_CancelSetInPlace = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Cauldron.CancelSetInPlace"));

	/**
	 * Input Tags
	 */
	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.LMB"));
	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.RMB"));
	GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.1"));
	GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.2"));
	GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.3"));
	GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.4"));
	GameplayTags.InputTag_5 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.5"));
	GameplayTags.InputTag_6 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.6"));
	GameplayTags.InputTag_7 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.7"));
	GameplayTags.InputTag_8 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.8"));
	GameplayTags.InputTag_9 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.9"));
	GameplayTags.InputTag_0 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.0"));
	GameplayTags.InputTag_Ctrl = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Ctrl"));
	GameplayTags.InputTag_Alt = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Alt"));
	GameplayTags.InputTag_Shift = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Shift"));
	// Ritual
	GameplayTags.Ritual_Position_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ritual.Position.1"));
	GameplayTags.Ritual_Position_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ritual.Position.2"));
	GameplayTags.Ritual_Position_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ritual.Position.3"));
	GameplayTags.Ritual_Position_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ritual.Position.4"));

	GameplayTags.Ritual_Input_Up = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ritual.Input.Up"));
	GameplayTags.Ritual_Input_Down = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ritual.Input.Down"));
	GameplayTags.Ritual_Input_Left = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ritual.Input.Left"));
	GameplayTags.Ritual_Input_Right = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ritual.Input.Right"));

	// --- Ritual Event Tags ---
	GameplayTags.Event_Ritual_InputSuccess = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.Ritual.InputSuccess"));
	GameplayTags.Event_Ritual_InputFailure = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.Ritual.InputFailure"));
	GameplayTags.Event_Ritual_SequenceSuccess = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.Ritual.SequenceSuccess"));
	GameplayTags.Event_Ritual_SequenceFailure = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.Ritual.SequenceFailure"));
	GameplayTags.Event_Ritual_CatastrophicFailure = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.Ritual.CatastrophicFailure"));
	GameplayTags.Event_Ritual_InputReceived = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.Ritual.InputReceived"));
	GameplayTags.Event_Ritual_TurnAdvanced = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.Ritual.TurnAdvanced"));

	
	// Cauldron Tags
	GameplayTags.Cauldron_State_Empty = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cauldron.State.Empty"));
	GameplayTags.Cauldron_State_Full = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cauldron.State.Full"));
	GameplayTags.Cauldron_State_PotionReady = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cauldron.State.PotionReady"));



	
}
