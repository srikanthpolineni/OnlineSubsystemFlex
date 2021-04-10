
#include "OnlineAsyncTaskFlexServer.h"
#include "OnlineSessionInterfaceFlex.h"
#include "FlexMasterServer.h"
#include "OnlineSessionSettings.h"

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
		FOnlineSessionInfoFlex* NewSessionInfo = (FOnlineSessionInfoFlex*) Session->SessionInfo.Get();
		FFlexMasterServerPtr masteServer = FFlexMasterServer::Get();
		if (masteServer->IsInitialized()) {
			FString IPAddr = NewSessionInfo->HostAddr->ToString(false);
			int32 Port = NewSessionInfo->HostAddr->GetPort();
			//masteServer->SendMessage("SESSIONCREATE", nullptr);
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
