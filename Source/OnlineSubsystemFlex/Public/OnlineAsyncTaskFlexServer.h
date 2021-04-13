#pragma once

#include "CoreMinimal.h"
#include "OnlineAsyncTaskManager.h"
#include "OnlineSubsystemFlex.h"



class FOnlineSessionAsyncTaskFlexCreateSession : public FOnlineAsyncTaskBasic<class FOnlineSubsystemFlex>
{

private:
	/** Has this request been started */
	bool bInit;
	/** Name of session being created */
	FName SessionName;

public:

	FOnlineSessionAsyncTaskFlexCreateSession(class FOnlineSubsystemFlex* InSubsystem, FName InSessionName) :
		FOnlineAsyncTaskBasic(InSubsystem),
		bInit (false),
		SessionName (InSessionName)
	{
	}

	virtual ~FOnlineSessionAsyncTaskFlexCreateSession()
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

class FOnlineSessionAsyncTaskFlexCreateSessionFinish : public FOnlineAsyncTaskBasic<class FOnlineSubsystemFlex>
{

public:
	FOnlineSessionAsyncTaskFlexCreateSessionFinish(class FOnlineSubsystemFlex* InSubsystem):
		FOnlineAsyncTaskBasic(InSubsystem)
	{
	}

	virtual ~FOnlineSessionAsyncTaskFlexCreateSessionFinish()
	{
	}

	virtual FString ToString() const override;

	virtual void Tick() override;

	virtual void Finalize() override;

	virtual void TriggerDelegates() override;

};


