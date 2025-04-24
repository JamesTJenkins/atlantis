// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Atlantis : ModuleRules
{
	public Atlantis(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "Text3D", "AIModule", "GameplayTasks" });

		PrivateDependencyModuleNames.AddRange(new string[] { "OnlineSubsystem", "OnlineSubsystemUtils", "OnlineSubsystemNull" });

		DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");
	}
}
