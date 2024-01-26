// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class global_2024 : ModuleRules
{
	public global_2024(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;


        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "AIModule", "InputCore", "DeveloperSettings", "NavigationSystem", "UMG" });

        PrivateIncludePaths.Add("global_2024");

        PrivateDependencyModuleNames.AddRange(new string[] { "CommonUI", "CommonInput", "EnhancedInput", "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
