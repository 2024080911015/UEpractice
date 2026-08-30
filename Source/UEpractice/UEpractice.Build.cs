// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UEpractice : ModuleRules
{
	public UEpractice(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"UEpractice",
			"UEpractice/Variant_Platforming",
			"UEpractice/Variant_Platforming/Animation",
			"UEpractice/Variant_Combat",
			"UEpractice/Variant_Combat/AI",
			"UEpractice/Variant_Combat/Animation",
			"UEpractice/Variant_Combat/Gameplay",
			"UEpractice/Variant_Combat/Interfaces",
			"UEpractice/Variant_Combat/UI",
			"UEpractice/Variant_SideScrolling",
			"UEpractice/Variant_SideScrolling/AI",
			"UEpractice/Variant_SideScrolling/Gameplay",
			"UEpractice/Variant_SideScrolling/Interfaces",
			"UEpractice/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
