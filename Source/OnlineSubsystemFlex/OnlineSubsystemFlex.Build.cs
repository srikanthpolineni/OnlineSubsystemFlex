// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class OnlineSubsystemFlex : ModuleRules
{
    public OnlineSubsystemFlex(ReadOnlyTargetRules Target) : base(Target)
    {
        PrivateDefinitions.Add("ONLINESUBSYSTEMFLEX_PACKAGE=1");

        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "OnlineSubsystemUtils",
                "OnlineSubsystemFlexLibrary"
            }
            );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "NetCore",
                "Engine",
                "Sockets",
                "Voice",
                "AudioMixer",
                "OnlineSubsystem",
                "Json",
                "PacketHandler",
                "Projects"
            }
            );

    }
}