#include "OnlineAsyncTaskManagerFlex.h"
#include "FlexMasterServer.h"
#include "DOM/JsonObject.h"


FOnlineAsyncTaskManagerFlex::FOnlineAsyncTaskManagerFlex(FOnlineSubsystemFlex* InOnlineSubsystem)
{
	FlexSubsystem = InOnlineSubsystem;
	FFlexMasterServerPtr masteServer = FFlexMasterServer::Get();
	masteServer->setObserver(this);
}

void FOnlineAsyncTaskManagerFlex::OnlineTick()
{
	//TODO: Run callbacks from flex core module
}

void FOnlineAsyncTaskManagerFlex::OnMasterServerObserve(FJsonObjectPtr& Json)
{

}
