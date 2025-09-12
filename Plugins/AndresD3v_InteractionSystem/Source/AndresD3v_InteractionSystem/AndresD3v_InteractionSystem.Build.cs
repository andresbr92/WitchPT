// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AndresD3v_InteractionSystem : ModuleRules
{
	public AndresD3v_InteractionSystem(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
	
		PublicDependencyModuleNames.AddRange(
			new[]
			{
				"Core", "SmartObjectsModule"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new[]
			{
				"CoreUObject",
				"NetCore",
				"Engine",
				"Slate",
				"SlateCore",
				"GameplayTags",
				"UMG",
				"TargetingSystem",
				"GameplayTasks",
				"GameplayAbilities",
				"GameplayBehaviorsModule",
				"SmartObjectsModule",
				"GameplayBehaviorSmartObjectsModule"
			}
		);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
