#pragma once

#include "CoreMinimal.h"
#include "Templates/Function.h"
#include "Templates/SharedPointer.h"
#include "Containers/UnrealString.h"
#include "Containers/Map.h"
#include "DOM/JsonObject.h"
#include "OnlineAsyncTaskManagerFlex.h"


class IWebSocket;
class FFlexMasterServer;
typedef TSharedPtr<class FFlexMasterServer, ESPMode::ThreadSafe> FFlexMasterServerPtr;
class FFlexMasterServer
{

public:
	FFlexMasterServer(){}
	static FFlexMasterServerPtr Get();

private:
	bool Init();
	void OnConnected();
	void OnConnectionError(const FString& Error);
	void OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean);
	void OnMessage(const FString& Msg);



public:
	~FFlexMasterServer();
	bool IsInitialized();
	void setObserver(FOnlineAsyncTaskManagerFlex* InObserver);

	bool SendMessage(FString eventType, TSharedPtr<FJsonObject>& jsonObject);



private:
	bool bIsInitialized = false;
	TSharedPtr<IWebSocket> WS;
	FDelegateHandle OnConnectedHandle;
	FDelegateHandle OnConnectionErrorHandle;
	FDelegateHandle OnClosedHandle;
	FDelegateHandle OnMessageHandle;
	FOnlineAsyncTaskManagerFlex* Observer;

	static FFlexMasterServerPtr Singleton;


};

