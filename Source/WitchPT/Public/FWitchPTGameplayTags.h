
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

	// Input Tags
	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;
	FGameplayTag InputTag_5;

private:
	static FWitchPTGameplayTags GameplayTags;
	
	
};
