// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Seelabor25_1 : ModuleRules
{
	public Seelabor25_1(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });
		PublicDependencyModuleNames.AddRange(new string[] { "ProceduralMeshComponent" });
		PublicDependencyModuleNames.AddRange(new string[] { "LidarPointCloudRuntime" });

		PublicDependencyModuleNames.AddRange( new string[] {"GDAL","UnrealGDAL" });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
