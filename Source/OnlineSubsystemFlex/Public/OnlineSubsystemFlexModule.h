// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

/**
* Online subsystem module class
*/
class FOnlineSubsystemFlexModule : public IModuleInterface
{

private:
	class FOnlineFactoryFlex* FlexFactory;

public:

	FOnlineSubsystemFlexModule()
		: FlexFactory(NULL)
	{}
	virtual ~FOnlineSubsystemFlexModule() {}

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	virtual bool SupportsDynamicReloading()
	{
		return false;
	}

	virtual bool SupportsAutomaticShutdown() override
	{
		return false;
	}
};
