// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FPSAssignment : ModuleRules
{
	public FPSAssignment(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"Http",
			"Json",
			"JsonUtilities",
            "EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"FPSAssignment",
			"FPSAssignment/Variant_Horror",
			"FPSAssignment/Variant_Horror/UI",
			"FPSAssignment/Variant_Shooter",
			"FPSAssignment/Variant_Shooter/AI",
			"FPSAssignment/Variant_Shooter/UI",
			"FPSAssignment/Variant_Shooter/Weapons"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
