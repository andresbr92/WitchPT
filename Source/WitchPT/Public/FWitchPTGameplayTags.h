
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

	// Ability Tags
	FGameplayTag Ability_Sprint;
	FGameplayTag Ability_Jump;
	FGameplayTag Ability_Crouch;

	// State Tags
	FGameplayTag State_Sprinting;
	FGameplayTag State_Crouching;
	FGameplayTag State_Dead;
	FGameplayTag State_Aim;

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

private:
	static FWitchPTGameplayTags GameplayTags;
	
	
};
