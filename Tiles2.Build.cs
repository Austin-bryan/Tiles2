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
			"Headers",
			"Headers/Coord",
			"Headers/Board",
			"Headers/Board/Populators",
			"Headers/Parsing",
			"Headers/Parsing/ParseStates",
			"Headers/Helper",
			"Headers/Tile",
			"Headers/TileModules",
			"Headers/Board/BoardActors",
			"Headers/Enums",
			"Headers/CreatorMenu",
			
			"Source",
			"Source/Coord",
			"Source/Board",
			"Source/Board/Populators",
			"Source/Parsing",
			"Source/Parsing/ParseStates",
			"Source/Helper",
			"Source/Tile",
			"Source/TileModules",
			"Source/Board/BoardActors",
			"Source/Enums",
			"Source/CreatorMenu",
			
			// "RuntimeMeshComponent",
			// "RuntimeMeshComponent/Public/Providers",
			// "RuntimeMeshComponent/Public/Components"
		});

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore",
			"UMG", "RuntimeMeshComponent",
		});
		PrivateDependencyModuleNames.AddRange(new string[] { "RD", "RD", "ProceduralMeshComponent", "Paper2D", "WorkspaceMenuStructure" });


		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
