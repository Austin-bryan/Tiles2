// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Tiles2 : ModuleRules
{
	public Tiles2(ReadOnlyTargetRules Target) : base(Target)
	{
		//PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		//string enginePath = Path.GetFullPath(BuildConfiguration.RelativeEnginePath);
		//string sourcePath = enginePath + "Source/Tiles2/";

		PrivateIncludePaths.AddRange(new string[]
		{
			"Coord",
			"Board",
			"Board/Populators",
			"Parsing",
			"Parsing/ParseStates",
			"P"
		});

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });
		PrivateDependencyModuleNames.AddRange(new string[] { "RD", "RD" });


		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
