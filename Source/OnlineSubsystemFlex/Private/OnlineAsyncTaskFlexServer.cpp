
#include "OnlineAsyncTaskFlexServer.h"
#include "OnlineSessionInterfaceFlex.h"
#include "FlexMasterServer.h"
#include "OnlineSessionSettings.h"
#include "DOM/JsonObject.h"
#include "HttpModule.h"
#include "PlatformHttp.h"
#include "Interfaces/IHttpResponse.h"

typedef TSharedPtr<class FFlexMasterServer, ESPMode::ThreadSafe> FFlexMasterServerPtr;

FString FOnlineSessionAsyncTaskFlexCreateSession::ToString() const
{
	return FString(TEXT("FOnlineSessionAsyncTaskFlexCreateSession"));
}

void FOnlineSessionAsyncTaskFlexCreateSession::Tick()
{
	FOnlineSessionFlexPtr SessionInt = StaticCastSharedPtr<FOnlineSessionFlex>(Subsystem->GetSessionInterface());
	if (!bInit) {
		FNamedOnlineSession* Session = (SessionInt.IsValid()) ? SessionInt->GetNamedSession(SessionName) : nullptr;
		FOnlineSessionInfoFlex* NewSessionInfo = (FOnlineSessionInfoFlex*)Session->SessionInfo.Get();
		FFlexMasterServerPtr masteServer = FFlexMasterServer::Get();
		if (masteServer->IsInitialized()) {
			FString IPAddr = NewSessionInfo->HostAddr->ToString(false);
			int32 Port = NewSessionInfo->HostAddr->GetPort();
			auto MessageJson = MakeShared<FJsonObject>();
			MessageJson->SetStringField(TEXT("ip"), IPAddr);
			MessageJson->SetNumberField(TEXT("port"), Port);
			masteServer->SendMessage(TEXT("SESSIONCREATE"), MessageJson);
			bInit = true;
		}

	}

	if (bInit && SessionInt->bFlexibleGameServerConnected)
	{
		bIsComplete = true;
		bWasSuccessful = true;
		SessionInt->OnCreateSessionComplete(SessionName, FOnlineError(bWasSuccessful));
	}
	else
	{
		if (GetElapsedTime() >= 15.0f)
		{
			bIsComplete = true;
			bWasSuccessful = false;
			SessionInt->OnCreateSessionComplete(SessionName, FOnlineError(bWasSuccessful));
		}
	}

}

void FOnlineSessionAsyncTaskFlexCreateSession::Finalize()
{
	UE_LOG_ONLINE(Log, TEXT("Flex Create Session Finished with status: %s."), bWasSuccessful ? TEXT("SUCCESSFUL") : TEXT("FAILED"));
}

void FOnlineSessionAsyncTaskFlexCreateSession::TriggerDelegates()
{

}

FString FOnlineSessionAsyncTaskFlexCreateSessionFinish::ToString() const
{
	return FString(TEXT("FOnlineSessionAsyncTaskFlexCreateSessionFinish"));
}

void FOnlineSessionAsyncTaskFlexCreateSessionFinish::Tick()
{

}

/**
 * This will triggered from WS callback
 */
void FOnlineSessionAsyncTaskFlexCreateSessionFinish::Finalize()
{
	FOnlineSessionFlexPtr SessionInt = StaticCastSharedPtr<FOnlineSessionFlex>(Subsystem->GetSessionInterface());
	SessionInt->bFlexibleGameServerConnected = true;
}

void FOnlineSessionAsyncTaskFlexCreateSessionFinish::TriggerDelegates()
{

}

FOnlineSessionAsyncTaskFlexFindServer::FOnlineSessionAsyncTaskFlexFindServer(class FOnlineSubsystemFlex* InSubsystem, const TSharedPtr<class FOnlineSessionSearch>& InSearchSettings, FOnAsyncFindServersComplete& InDelegates)
	:FOnlineAsyncTaskBasic(InSubsystem)
{
	bInit = false;
	SearchSettings = InSearchSettings;
	FindServersCompleteDelegates = InDelegates;
	HttpClient = FHttpModule::Get().CreateRequest();
	HttpClient->SetVerb(TEXT("GET"));
	HttpClient->SetURL(TEXT("http://localhost:8080/api/clients"));
	HttpClient->OnProcessRequestComplete().BindLambda(
		[=](FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
		{
			//TODO:
		});
	HttpClient->ProcessRequest();
	bInit = true;
}

void FOnlineSessionAsyncTaskFlexFindServer::Tick()
{

}

FString FOnlineSessionAsyncTaskFlexFindServer::ToString() const
{
	return FString(TEXT("FOnlineSessionAsyncTaskFlexFindServer"));
}

void FOnlineSessionAsyncTaskFlexFindServer::Finalize()
{

}

void FOnlineSessionAsyncTaskFlexFindServer::TriggerDelegates()
{
	if (FindServersCompleteDelegates.IsBound())
	{
		FindServersCompleteDelegates.Broadcast(bWasSuccessful);
	}
}

FString FOnlineSessionAsyncTaskFlexStartSession::ToString() const
{
	return FString(TEXT("FOnlineSessionAsyncTaskFlexStartSession"));
}

void FOnlineSessionAsyncTaskFlexStartSession::Tick()
{
	bIsComplete = true;
	bWasSuccessful = true;
}

void FOnlineSessionAsyncTaskFlexStartSession::Finalize()
{
	UE_LOG_ONLINE(Log, TEXT("Flex Start Session Finished with status: %s."), bWasSuccessful ? TEXT("SUCCESSFUL") : TEXT("FAILED"));
}

void FOnlineSessionAsyncTaskFlexStartSession::TriggerDelegates()
{

}
