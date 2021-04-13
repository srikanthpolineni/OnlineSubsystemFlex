#include "OnlineAsyncTaskManagerFlex.h"
#include "FlexMasterServer.h"
#include "DOM/JsonObject.h"
#include "OnlineAsyncTaskFlexServer.h"


FOnlineAsyncTaskManagerFlex::FOnlineAsyncTaskManagerFlex(FOnlineSubsystemFlex* InOnlineSubsystem)
{
	FlexSubsystem = InOnlineSubsystem;
}

bool FOnlineAsyncTaskManagerFlex::InitServer()
{
	masteServer = FFlexMasterServer::Get();
	masteServer->setObserver(this);
	masteServer->Init();
	return true;
}

bool FOnlineAsyncTaskManagerFlex::InitClient()
{
	return true;
}

void FOnlineAsyncTaskManagerFlex::OnlineTick()
{
	//TODO: Run callbacks from flex core module
}

void FOnlineAsyncTaskManagerFlex::OnMasterServerObserve(FJsonObjectPtr& JsonMsg)
{
	FString ActionType;
	if (!JsonMsg->TryGetStringField(TEXT("action"), ActionType))
	{
		UE_LOG_ONLINE(Error, TEXT("Cannot find `action` field in WS message"));
	}
	if (ActionType == TEXT("SESSIONCREATE"))
	{
		uint32 ActionCode;
		if (!JsonMsg->TryGetNumberField(TEXT("code"), ActionCode))
		{
			UE_LOG_ONLINE(Error, TEXT("Failed to get `code`from `JsonMsg` message\n%s"), *ToString(JsonMsg));
		}
		if (ActionCode == 200 || ActionCode == 409)
		{
			FOnlineSessionAsyncTaskFlexCreateSessionFinish* NewTask = new FOnlineSessionAsyncTaskFlexCreateSessionFinish(FlexSubsystem);
			AddToOutQueue(NewTask);
		}
	}

}
