// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class demo_ue5_cxx : ModuleRules
{
	public demo_ue5_cxx(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" ,"UMG","Slate","SlateCore", "OpenSSL"});

		PrivateDependencyModuleNames.AddRange(new string[] { "LibhvModule" });


		PublicIncludePaths.AddRange(new string[]
		{
			"demo_ue5_cxx",
			"demo_ue5_cxx/kbengine_cxx_plugins"
		});

		PrivateIncludePaths.AddRange(new string[]
		{
			"demo_ue5_cxx",
			"demo_ue5_cxx/kbengine_cxx_plugins"
		});


        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
