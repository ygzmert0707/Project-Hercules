// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PrjctHerculesPrttype : ModuleRules
{
	public PrjctHerculesPrttype(ReadOnlyTargetRules Target) : base(Target)
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
			"PrjctHerculesPrttype",
			"PrjctHerculesPrttype/Variant_Platforming",
			"PrjctHerculesPrttype/Variant_Platforming/Animation",
			"PrjctHerculesPrttype/Variant_Combat",
			"PrjctHerculesPrttype/Variant_Combat/AI",
			"PrjctHerculesPrttype/Variant_Combat/Animation",
			"PrjctHerculesPrttype/Variant_Combat/Gameplay",
			"PrjctHerculesPrttype/Variant_Combat/Interfaces",
			"PrjctHerculesPrttype/Variant_Combat/UI",
			"PrjctHerculesPrttype/Variant_SideScrolling",
			"PrjctHerculesPrttype/Variant_SideScrolling/AI",
			"PrjctHerculesPrttype/Variant_SideScrolling/Gameplay",
			"PrjctHerculesPrttype/Variant_SideScrolling/Interfaces",
			"PrjctHerculesPrttype/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
