
#include "OnlineAsyncTaskFlexServer.h"
#include "OnlineSessionInterfaceFlex.h"
#include "FlexMasterServer.h"
#include "OnlineSessionSettings.h"
#include "DOM/JsonObject.h"

typedef TSharedPtr<class FFlexMasterServer, ESPMode::ThreadSafe> FFlexMasterServerPtr;

FString FOnlineSessionAsyncTaskFlexCreateSession::ToString() const
{
	return FString();
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
	}
	else
	{
		if (GetElapsedTime() >= 15.0f)
		{
			bIsComplete = true;
			bWasSuccessful = false;
		}
	}

}

void FOnlineSessionAsyncTaskFlexCreateSession::Finalize()
{
}

void FOnlineSessionAsyncTaskFlexCreateSession::TriggerDelegates()
{

}

FString FOnlineSessionAsyncTaskFlexCreateSessionFinish::ToString() const
{
	return FString();
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
