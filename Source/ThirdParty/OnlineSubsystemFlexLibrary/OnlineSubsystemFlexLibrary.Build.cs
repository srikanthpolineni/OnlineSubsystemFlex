// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class OnlineSubsystemFlexLibrary : ModuleRules
{
	public OnlineSubsystemFlexLibrary(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			// Add the import library
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "x64", "Release", "OnlineSubsystemFlexLibrary.lib"));

			// Delay-load the DLL, so we can load it from the right place first
			PublicDelayLoadDLLs.Add("OnlineSubsystemFlexLibrary.dll");

			// Ensure that the DLL is staged along with the executable
			RuntimeDependencies.Add("$(PluginDir)/Binaries/ThirdParty/OnlineSubsystemFlexLibrary/Win64/OnlineSubsystemFlexLibrary.dll");
        }
        else if (Target.Platform == UnrealTargetPlatform.Mac)
        {
            PublicDelayLoadDLLs.Add(Path.Combine(ModuleDirectory, "Mac", "Release", "libOnlineSubsystemFlexLibrary.dylib"));
            RuntimeDependencies.Add("$(PluginDir)/Source/ThirdParty/OnlineSubsystemFlexLibrary/Mac/Release/libOnlineSubsystemFlexLibrary.dylib");
        }
	}
}
