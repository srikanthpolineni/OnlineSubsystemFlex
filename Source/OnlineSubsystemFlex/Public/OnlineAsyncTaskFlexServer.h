#pragma once

#include "CoreMinimal.h"
#include "OnlineAsyncTaskManager.h"
#include "OnlineSubsystemFlex.h"



typedef void (*FlexMasterAPICall_t)();

class FOnlineSessionAsyncTaskFlexCreateServer : public FOnlineAsyncTaskBasic<class FOnlineSubsystemFlex>
{

private:
	/** Has this request been started */
	bool bInit;
	/** Name of session being created */
	FName SessionName;
	FlexMasterAPICall_t CallbackHandle;

public:

	FOnlineSessionAsyncTaskFlexCreateServer(class FOnlineSubsystemFlex* InSubsystem, FName InSessionName, FlexMasterAPICall_t InCallbackHandle) :
		FOnlineAsyncTaskBasic(InSubsystem),
		bInit (false),
		SessionName (InSessionName),
		CallbackHandle(InCallbackHandle)
	{
	}

	virtual ~FOnlineSessionAsyncTaskFlexCreateServer()
	{
	}

	/**
	 *	Get a human readable description of task
	 */
	virtual FString ToString() const override;

	/**
	 * Give the async task time to do its work
	 * Can only be called on the async task manager thread
	 */
	virtual void Tick() override;

	/**
	 * Give the async task a chance to marshal its data back to the game thread
	 * Can only be called on the game thread by the async task manager
	 */
	virtual void Finalize() override;

	/**
	 *	Async task is given a chance to trigger it's delegates
	 */
	virtual void TriggerDelegates() override;

};

