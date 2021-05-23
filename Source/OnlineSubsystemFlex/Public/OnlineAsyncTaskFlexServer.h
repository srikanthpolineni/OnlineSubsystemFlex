#pragma once

#include "CoreMinimal.h"
#include "OnlineAsyncTaskManager.h"
#include "OnlineSubsystemFlex.h"
#include "OnlineSessionSettings.h"



class IHttpClientObserver
{
	virtual void OnSuccessResponse(TSharedPtr<FJsonObject> JsonObject);

	virtual void OnFailedResponse(int32 StatusCode, FString Message);
};


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

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAsyncFindServersComplete, bool);
typedef FOnAsyncFindServersComplete::FDelegate FOnAsyncFindServersCompleteDelegate;

class FOnlineSessionAsyncTaskFlexFindServer : public FOnlineAsyncTaskBasic<class FOnlineSubsystemFlex>
{

private:
	bool bInit;
	TSharedPtr<class FOnlineSessionSearch> SearchSettings;
	FOnAsyncFindServersComplete FindServersCompleteDelegates;
	TSharedPtr<class IHttpRequest, ESPMode::ThreadSafe> HttpClient;

public:

	FOnlineSessionAsyncTaskFlexFindServer(class FOnlineSubsystemFlex* InSubsystem, const TSharedPtr<class FOnlineSessionSearch>& InSearchSettings, FOnAsyncFindServersComplete& InDelegates);


	virtual ~FOnlineSessionAsyncTaskFlexFindServer()
	{
	}

	virtual void Tick() override;


	virtual FString ToString() const override;


	virtual void Finalize() override;


	virtual void TriggerDelegates() override;

};


