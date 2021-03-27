// Copyright Epic Games, Inc. All Rights Reserved.
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "OnlineSubsystemFlexModule.h"
#include "OnlineSubsystemModule.h"
#include "OnlineSubsystemNames.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemFlex.h"
#include "Interfaces/IPluginManager.h"
#include "OnlineSubsystemFlexLibrary/OnlineSubsystemFlexLibrary.h"

IMPLEMENT_MODULE(FOnlineSubsystemFlexModule, OnlineSubsystemFlex)

class FOnlineFactoryFlex : public IOnlineFactory {

private:
	static FOnlineSubsystemFlexPtr FlexSingleton;

	virtual void DestroySubsystem()
	{
		if (FlexSingleton.IsValid())
		{
			FlexSingleton->Shutdown();
			FlexSingleton = nullptr;
		}
	}

public:
	FOnlineFactoryFlex() {}
	virtual ~FOnlineFactoryFlex()
	{
		DestroySubsystem();
	}

	virtual IOnlineSubsystemPtr CreateSubsystem(FName InstanceName)
	{
		if (!FlexSingleton.IsValid())
		{
			FlexSingleton = MakeShared<FOnlineSubsystemFlex, ESPMode::ThreadSafe>(InstanceName);
			if (FlexSingleton->IsEnabled())
			{
				if (!FlexSingleton->Init())
				{
					UE_LOG_ONLINE(Warning, TEXT("Flex API failed to initialize!"));
					DestroySubsystem();
				}
			}
			else
			{
				UE_CLOG_ONLINE(IsRunningDedicatedServer() || IsRunningGame(), Warning, TEXT("Flex API disabled!"));
				DestroySubsystem();
			}
			return FlexSingleton;
		}
		UE_LOG_ONLINE(Warning, TEXT("Can't create more than one instance of Flex online subsystem!"));
		return nullptr;
	}

};

FOnlineSubsystemFlexPtr FOnlineFactoryFlex::FlexSingleton = nullptr;

void FOnlineSubsystemFlexModule::StartupModule()
{

	FlexFactory = new FOnlineFactoryFlex();
	FOnlineSubsystemModule& OSS = FModuleManager::GetModuleChecked<FOnlineSubsystemModule>("OnlineSubsystem");
	OSS.RegisterPlatformService(FLEX_SUBSYSTEM, FlexFactory);
}

void FOnlineSubsystemFlexModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	// Free the dll handle
	FPlatformProcess::FreeDllHandle(OnlineSubsystemFlexLibraryHandle);
	OnlineSubsystemFlexLibraryHandle = nullptr;
}

