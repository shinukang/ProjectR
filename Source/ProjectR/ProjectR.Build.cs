// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectR : ModuleRules
{
	public ProjectR(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new[] { "Core", "CoreUObject", "Engine", "InputCore", "NavigationSystem", "AIModule", "GameplayTasks", "PhysicsCore", "Niagara", "EnhancedInput" });

        PrivateDependencyModuleNames.AddRange(new[] { "Slate", "SlateCore" });
    }
}
