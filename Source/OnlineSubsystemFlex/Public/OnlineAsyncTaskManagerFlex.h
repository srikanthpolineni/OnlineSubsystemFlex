#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystem.h"
#include "OnlineAsyncTaskManager.h"
#include "DOM/JsonObject.h"
#include "Policies/PrettyJsonPrintPolicy.h"
#include "Policies/CondensedJsonPrintPolicy.h"
#include "Serialization/JsonSerializer.h"

class FJsonObject;
class FFlexMasterServer;
typedef TSharedPtr<class FFlexMasterServer, ESPMode::ThreadSafe> FFlexMasterServerPtr;
class FOnlineAsyncTaskManagerFlex : public FOnlineAsyncTaskManager
{


protected:

	/** Cached reference to the main online subsystem */
	class FOnlineSubsystemFlex* FlexSubsystem;

private:
	FFlexMasterServerPtr masteServer;

public:
	FOnlineAsyncTaskManagerFlex(FOnlineSubsystemFlex* InOnlineSubsystem);
	~FOnlineAsyncTaskManagerFlex()
	{
	}

	bool InitServer();

	bool InitClient();

	// FOnlineAsyncTaskManager
	virtual void OnlineTick() override;

	using FJsonObjectPtr = TSharedPtr<FJsonObject>;
	void OnMasterServerObserve(FJsonObjectPtr& Json);

private:

	inline FString ToString(const TSharedPtr<FJsonObject>& JsonObj, bool bPretty = true)
	{
		FString Res;
		if (bPretty)
		{
			auto JsonWriter = TJsonWriterFactory<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>::Create(&Res);
			FJsonSerializer::Serialize(JsonObj.ToSharedRef(), JsonWriter);
		}
		else
		{
			auto JsonWriter = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&Res);
			FJsonSerializer::Serialize(JsonObj.ToSharedRef(), JsonWriter);
		}
		return Res;
	}



};


