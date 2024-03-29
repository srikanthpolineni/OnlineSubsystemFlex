#include "OnlineSubsystemFlex.h"
#include "FlexMasterServer.h"

IOnlineEventsPtr FOnlineSubsystemFlex::GetEventsInterface() const
{
	return nullptr;
}

IOnlineAchievementsPtr FOnlineSubsystemFlex::GetAchievementsInterface() const
{
	return nullptr;
}

IOnlineSharingPtr FOnlineSubsystemFlex::GetSharingInterface() const
{
	return nullptr;
}

IOnlineUserPtr FOnlineSubsystemFlex::GetUserInterface() const
{
	return nullptr;
}

IOnlineMessagePtr FOnlineSubsystemFlex::GetMessageInterface() const
{
	return nullptr;
}

IOnlinePresencePtr FOnlineSubsystemFlex::GetPresenceInterface() const
{
	return nullptr;
}

IOnlineChatPtr FOnlineSubsystemFlex::GetChatInterface() const
{
	return nullptr;
}

IOnlineStatsPtr FOnlineSubsystemFlex::GetStatsInterface() const
{
	return nullptr;
}

IOnlineTurnBasedPtr FOnlineSubsystemFlex::GetTurnBasedInterface() const
{
	return nullptr;
}

IOnlineTournamentPtr FOnlineSubsystemFlex::GetTournamentInterface() const
{
	return nullptr;
}

bool FOnlineSubsystemFlex::IsLocalPlayer(const FUniqueNetId& UniqueId) const
{
	return false;
}

bool FOnlineSubsystemFlex::Init()
{
	const bool bIsServer = IsRunningDedicatedServer();
	const bool bIsGame = IsRunningGame();

	OnlineAsyncTaskThreadRunnable = new FOnlineAsyncTaskManagerFlex(this);
	check(OnlineAsyncTaskThreadRunnable);
	OnlineAsyncTaskThread = FRunnableThread::Create(OnlineAsyncTaskThreadRunnable, *FString::Printf(TEXT("OnlineAsyncTaskThreadFlex %s"), *InstanceName.ToString()), 128 * 1024, TPri_Normal);
	check(OnlineAsyncTaskThread);
	UE_LOG_ONLINE(Verbose, TEXT("Created thread (ID:%d)."), OnlineAsyncTaskThread->GetThreadID());

	if (bIsGame) {
		OnlineAsyncTaskThreadRunnable->InitClient();
	}
	if (bIsServer) {
		OnlineAsyncTaskThreadRunnable->InitServer();
	}



	SessionInterface = MakeShareable(new FOnlineSessionFlex(this));


	return bIsServer || bIsGame;
}

bool FOnlineSubsystemFlex::Shutdown()
{
	return true;
}

bool FOnlineSubsystemFlex::Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar)
{
	return false;
}

bool FOnlineSubsystemFlex::IsEnabled() const
{
	return true;
}

FString FOnlineSubsystemFlex::GetAppId() const
{
	return FString();
}

FText FOnlineSubsystemFlex::GetOnlineServiceName() const
{
	return FText();
}

bool FOnlineSubsystemFlex::Tick(float DeltaTime)
{
	if (!FOnlineSubsystemImpl::Tick(DeltaTime))
	{
		return false;
	}

	if (OnlineAsyncTaskThreadRunnable)
	{
		OnlineAsyncTaskThreadRunnable->GameTick();
	}

	if (SessionInterface.IsValid())
	{
		SessionInterface->Tick(DeltaTime);
	}

	if (VoiceInterface.IsValid() && bVoiceInterfaceInitialized)
	{
		VoiceInterface->Tick(DeltaTime);
	}

	return true;
}

IOnlineSessionPtr FOnlineSubsystemFlex::GetSessionInterface() const
{
	return SessionInterface;
}

IOnlineFriendsPtr FOnlineSubsystemFlex::GetFriendsInterface() const
{
	return nullptr;
}

IOnlinePartyPtr FOnlineSubsystemFlex::GetPartyInterface() const
{
	return nullptr;
}

IOnlineGroupsPtr FOnlineSubsystemFlex::GetGroupsInterface() const
{
	return nullptr;
}

IOnlineSharedCloudPtr FOnlineSubsystemFlex::GetSharedCloudInterface() const
{
	return nullptr;
}

IOnlineUserCloudPtr FOnlineSubsystemFlex::GetUserCloudInterface() const
{
	return nullptr;
}

IOnlineLeaderboardsPtr FOnlineSubsystemFlex::GetLeaderboardsInterface() const
{
	return nullptr;
}

IOnlineVoicePtr FOnlineSubsystemFlex::GetVoiceInterface() const
{
	return VoiceInterface;
}

IOnlineExternalUIPtr FOnlineSubsystemFlex::GetExternalUIInterface() const
{
	return nullptr;
}

IOnlineTimePtr FOnlineSubsystemFlex::GetTimeInterface() const
{
	return nullptr;
}

IOnlineIdentityPtr FOnlineSubsystemFlex::GetIdentityInterface() const
{
	return nullptr;
}

IOnlineTitleFilePtr FOnlineSubsystemFlex::GetTitleFileInterface() const
{
	return nullptr;
}

IOnlineEntitlementsPtr FOnlineSubsystemFlex::GetEntitlementsInterface() const
{
	return nullptr;
}

void FOnlineSubsystemFlex::QueueAsyncTask(FOnlineAsyncTask* AsyncTask)
{
	check(OnlineAsyncTaskThreadRunnable);
	OnlineAsyncTaskThreadRunnable->AddToInQueue(AsyncTask);
}
