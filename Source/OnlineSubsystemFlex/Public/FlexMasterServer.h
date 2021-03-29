#pragma once

#include "CoreMinimal.h"
#include "Templates/Function.h"
#include "Templates/SharedPointer.h"
#include "Containers/UnrealString.h"
#include "Containers/Map.h"
#include "DOM/JsonObject.h"
#include "Misc/ScopeLock.h"


class IWebSocket;

typedef TSharedPtr<class FFlexMasterServer, ESPMode::ThreadSafe> FFlexMasterServerPtr;
class FFlexMasterServer
{

public:
	FFlexMasterServer(FFlexMasterServer const&) = delete;
	FFlexMasterServer& operator=(FFlexMasterServer const&) = delete;

	static FFlexMasterServerPtr Instance()
	{
		FScopeLock ScopeLock(&FlexMasterServerLock);
		static FFlexMasterServerPtr  ins{ MakeShareable(new FFlexMasterServer) };
		return ins;
	}


private:
	FFlexMasterServer();
	bool Init();
	void OnConnected();
	void OnConnectionError(const FString& Error);
	void OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean);
	void OnMessage(const FString& Msg);

protected:
	static FCriticalSection FlexMasterServerLock;
	


public:
	~FFlexMasterServer();
	bool IsInitialized();

	bool Notify(FString str);



private:
	bool bIsInitialized;
	TSharedPtr<IWebSocket> WS;
	FDelegateHandle OnConnectedHandle;
	FDelegateHandle OnConnectionErrorHandle;
	FDelegateHandle OnClosedHandle;
	FDelegateHandle OnMessageHandle;


};

