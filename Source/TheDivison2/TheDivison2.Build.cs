// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TheDivison2 : ModuleRules
{
	public TheDivison2(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "AIModule", "NavigationSystem" });
	}
}
