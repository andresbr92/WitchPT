#include "FWitchPTGameplayTags.h"

#include "GameplayTagsManager.h"

FWitchPTGameplayTags FWitchPTGameplayTags::GameplayTags;

void FWitchPTGameplayTags::InitializeGameplayTags()
{
	GameplayTags.Attributes_Primary_Health = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Vital.Health"));
	GameplayTags.Attributes_Primary_Age = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Vital.Age"));
	GameplayTags.Attributes_Secondary_Mana = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Vital.Mana"));
	GameplayTags.Attributes_Secondary_Stamina = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Vital.Stamina"));
	GameplayTags.Ability_Sprint = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Sprint"));
	GameplayTags.Ability_Jump = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Jump"));
	GameplayTags.Ability_Crouch = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Crouch"));
	GameplayTags.State_Sprinting = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("State.Sprinting"));
	GameplayTags.State_Crouching = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("State.Crouching"));
	GameplayTags.State_Dead = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("State.Dead"));



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
}
