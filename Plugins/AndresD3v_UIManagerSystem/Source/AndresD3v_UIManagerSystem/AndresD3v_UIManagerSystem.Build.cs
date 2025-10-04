// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AndresD3v_UIManagerSystem : ModuleRules
{
	public AndresD3v_UIManagerSystem(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CommonUI"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"ApplicationCore",
				"EnhancedInput",
				"PropertyPath",
				"GameplayTags",
				"UMG",
				"InputCore",
				"CommonInput",
				"DeveloperSettings",
				"ModularGameplay"
			}
		);
	}
}
