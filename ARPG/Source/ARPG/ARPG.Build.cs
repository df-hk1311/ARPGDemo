// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ARPG : ModuleRules
{
	public ARPG(ReadOnlyTargetRules Target) : base(Target)
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
			"ARPG",
			"ARPG/Variant_Platforming",
			"ARPG/Variant_Platforming/Animation",
			"ARPG/Variant_Combat",
			"ARPG/Variant_Combat/AI",
			"ARPG/Variant_Combat/Animation",
			"ARPG/Variant_Combat/Gameplay",
			"ARPG/Variant_Combat/Interfaces",
			"ARPG/Variant_Combat/UI",
			"ARPG/Variant_SideScrolling",
			"ARPG/Variant_SideScrolling/AI",
			"ARPG/Variant_SideScrolling/Gameplay",
			"ARPG/Variant_SideScrolling/Interfaces",
			"ARPG/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
