#include "FlexMasterServer.h"
#include "WebSocketsModule.h"
#include "IWebSocket.h"
#include "DOM/JsonObject.h"
#include "Serialization/JsonSerializer.h"

FFlexMasterServer::FFlexMasterServer()
{
	bIsInitialized = false;
}

bool FFlexMasterServer::Init()
{
	bool result = true;

	FString Url;
	GConfig->GetString(TEXT("OnlineSubsystemFlex"), TEXT("MasterServerWS"), Url, GEngineIni);

	WS = FWebSocketsModule::Get().CreateWebSocket(Url, TEXT("ws"));
	OnConnectedHandle = WS->OnConnected().AddLambda([this]() { OnConnected(); });
	OnConnectionErrorHandle = WS->OnConnectionError().AddLambda([this](const FString& Error) { OnConnectionError(Error); });
	OnClosedHandle = WS->OnClosed().AddLambda([this](int32 StatusCode, const FString& Reason, bool bWasClean) { OnClosed(StatusCode, Reason, bWasClean); });
	OnMessageHandle = WS->OnMessage().AddLambda([this](const FString& Msg) { OnMessage(Msg); });

	WS->Connect();

	return result;

}

void FFlexMasterServer::OnConnected()
{
	bIsInitialized = true;
}

void FFlexMasterServer::OnConnectionError(const FString& Error)
{
	bIsInitialized = false;
}

void FFlexMasterServer::OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean)
{
	bIsInitialized = false;
}

void FFlexMasterServer::OnMessage(const FString& Msg)
{

}

FFlexMasterServer::~FFlexMasterServer()
{
	if (!WS)
	{
		return;
	}

	WS->OnConnected().Remove(OnConnectedHandle);
	WS->OnConnectionError().Remove(OnConnectionErrorHandle);
	WS->OnClosed().Remove(OnClosedHandle);
	WS->OnMessage().Remove(OnMessageHandle);

	WS->Close();
}

bool FFlexMasterServer::IsInitialized()
{
	return bIsInitialized;
}
