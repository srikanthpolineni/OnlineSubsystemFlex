#include "OnlineAsyncTaskManagerFlex.h"
#include "FlexMasterServer.h"
#include "DOM/JsonObject.h"


FOnlineAsyncTaskManagerFlex::FOnlineAsyncTaskManagerFlex(FOnlineSubsystemFlex* InOnlineSubsystem)
{
	FlexSubsystem = InOnlineSubsystem;
	masteServer = FFlexMasterServer::Get();
	masteServer->setObserver(this);
	masteServer->Init();
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

}
