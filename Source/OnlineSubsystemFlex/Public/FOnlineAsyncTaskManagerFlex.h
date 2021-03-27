#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystem.h"
#include "OnlineAsyncTaskManager.h"

class FOnlineAsyncTaskManagerFlex : public FOnlineAsyncTaskManager
{


protected:

	/** Cached reference to the main online subsystem */
	class FOnlineSubsystemFlex* FlexSubsystem;


public:
	FOnlineAsyncTaskManagerFlex(class FOnlineSubsystemFlex* InOnlineSubsystem):
		FlexSubsystem(InOnlineSubsystem)
	{}
	~FOnlineAsyncTaskManagerFlex()
	{
	}

	// FOnlineAsyncTaskManager
	virtual void OnlineTick() override;

};

