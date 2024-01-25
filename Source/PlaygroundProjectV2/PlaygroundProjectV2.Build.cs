// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PlaygroundProjectV2 : ModuleRules
{
	public PlaygroundProjectV2(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Niagara", "SlateCore"});
	}
}