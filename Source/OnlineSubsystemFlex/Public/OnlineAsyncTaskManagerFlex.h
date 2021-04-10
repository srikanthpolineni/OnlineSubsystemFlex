#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystem.h"
#include "OnlineAsyncTaskManager.h"

class FJsonObject;
class FFlexMasterServer;
typedef TSharedPtr<class FFlexMasterServer, ESPMode::ThreadSafe> FFlexMasterServerPtr;
class FOnlineAsyncTaskManagerFlex : public FOnlineAsyncTaskManager
{


protected:

	/** Cached reference to the main online subsystem */
	class FOnlineSubsystemFlex* FlexSubsystem;

private:
	FFlexMasterServerPtr masteServer;

public:
	FOnlineAsyncTaskManagerFlex(FOnlineSubsystemFlex* InOnlineSubsystem);
	~FOnlineAsyncTaskManagerFlex()
	{
	}

	// FOnlineAsyncTaskManager
	virtual void OnlineTick() override;

	using FJsonObjectPtr = TSharedPtr<FJsonObject>;
	void OnMasterServerObserve(FJsonObjectPtr& Json);



};


