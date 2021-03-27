#include "OnlineSessionInterfaceFlex.h"
#include "SocketSubsystem.h"
#include "OnlineSubsystemFlex.h"
#include "OnlineSubsystemUtils.h"


#pragma region FOnlineSessionInfoFlex


FOnlineSessionInfoFlex::FOnlineSessionInfoFlex(EFlexSession::Type InSessionType) :
	SessionType(InSessionType),
	HostAddr(nullptr),
	FlexP2PAddr(nullptr),
	SessionId((uint64)0),
	ConnectionMethod((InSessionType == EFlexSession::LANSession) ? FFlexConnectionMethod::Direct : FFlexConnectionMethod::None)
{
}

FOnlineSessionInfoFlex::FOnlineSessionInfoFlex(EFlexSession::Type InSessionType, const FUniqueNetIdFlex& InSessionId) :
	SessionType(InSessionType),
	HostAddr(nullptr),
	FlexP2PAddr(nullptr),
	SessionId(InSessionId),
	ConnectionMethod(FFlexConnectionMethod::None)
{
}

void FOnlineSessionInfoFlex::Init(const FOnlineSubsystemFlex& Subsystem)
{
	// Read the IP from the system
	bool bCanBindAll;
	HostAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, bCanBindAll);

	// The below is a workaround for systems that set hostname to a distinct address from 127.0.0.1 on a loopback interface.
	// See e.g. https://www.debian.org/doc/manuals/debian-reference/ch05.en.html#_the_hostname_resolution
	// and http://serverfault.com/questions/363095/why-does-my-hostname-appear-with-the-address-127-0-1-1-rather-than-127-0-0-1-in
	// Since we bind to 0.0.0.0, we won't answer on 127.0.1.1, so we need to advertise ourselves as 127.0.0.1 for any other loopback address we may have.
	uint32 HostIp = 0;
	HostAddr->GetIp(HostIp); // will return in host order
	// if this address is on loopback interface, advertise it as 127.0.0.1
	if ((HostIp & 0xff000000) == 0x7f000000)
	{
		HostAddr->SetIp(0x7f000001);	// 127.0.0.1
	}

	// Now set the port that was configured
	HostAddr->SetPort(GetPortFromNetDriver(Subsystem.GetInstanceName()));

	FGuid OwnerGuid;
	FPlatformMisc::CreateGuid(OwnerGuid);
	SessionId = FUniqueNetIdFlex(OwnerGuid.ToString());
}

void FOnlineSessionInfoFlex::InitLAN()
{
}

#pragma endregion FOnlineSessionInfoFlex

void FOnlineSessionFlex::TickLanTasks(float DeltaTime)
{
}

void FOnlineSessionFlex::TickPendingInvites(float DeltaTime)
{
}

uint32 FOnlineSessionFlex::CreateLobbySession(int32 HostingPlayerNum, FNamedOnlineSession* Session)
{
	return uint32();
}

uint32 FOnlineSessionFlex::CreateInternetSession(int32 HostingPlayerNum, FNamedOnlineSession* Session)
{
	return uint32();
}

uint32 FOnlineSessionFlex::JoinLobbySession(int32 PlayerNumber, FNamedOnlineSession* Session, const FOnlineSession* SearchSession)
{
	return uint32();
}

uint32 FOnlineSessionFlex::JoinInternetSession(int32 PlayerNumber, FNamedOnlineSession* Session, const FOnlineSession* SearchSession)
{
	return uint32();
}

uint32 FOnlineSessionFlex::EndInternetSession(FNamedOnlineSession* Session)
{
	return uint32();
}

uint32 FOnlineSessionFlex::DestroyLobbySession(FNamedOnlineSession* Session, const FOnDestroySessionCompleteDelegate& CompletionDelegate)
{
	return uint32();
}

uint32 FOnlineSessionFlex::DestroyInternetSession(FNamedOnlineSession* Session, const FOnDestroySessionCompleteDelegate& CompletionDelegate)
{
	return uint32();
}

uint32 FOnlineSessionFlex::CreateLANSession(int32 HostingPlayerNum, FNamedOnlineSession* Session)
{
	return uint32();
}

uint32 FOnlineSessionFlex::JoinLANSession(int32 PlayerNum, FNamedOnlineSession* Session, const FOnlineSession* SearchSession)
{
	return uint32();
}

uint32 FOnlineSessionFlex::FindInternetSession(const TSharedRef<FOnlineSessionSearch>& SearchSettings)
{
	return uint32();
}

uint32 FOnlineSessionFlex::FindLANSession(const TSharedRef<FOnlineSessionSearch>& SearchSettings)
{
	return uint32();
}

void FOnlineSessionFlex::AppendSessionToPacket(FNboSerializeToBufferSteam& Packet, FOnlineSession* Session)
{
}

void FOnlineSessionFlex::AppendSessionSettingsToPacket(FNboSerializeToBufferSteam& Packet, FOnlineSessionSettings* SessionSettings)
{
}

void FOnlineSessionFlex::ReadSessionFromPacket(FNboSerializeFromBufferSteam& Packet, FOnlineSession* Session)
{
}

void FOnlineSessionFlex::ReadSettingsFromPacket(FNboSerializeFromBufferSteam& Packet, FOnlineSessionSettings& SessionSettings)
{
}

void FOnlineSessionFlex::OnValidQueryPacketReceived(uint8* PacketData, int32 PacketLength, uint64 ClientNonce)
{
}

void FOnlineSessionFlex::OnValidResponsePacketReceived(uint8* PacketData, int32 PacketLength)
{
}

void FOnlineSessionFlex::OnLANSearchTimeout()
{
}

void FOnlineSessionFlex::RegisterVoice(const FUniqueNetId& PlayerId)
{
}

void FOnlineSessionFlex::UnregisterVoice(const FUniqueNetId& PlayerId)
{
}

void FOnlineSessionFlex::Tick(float DeltaTime)
{
}

void FOnlineSessionFlex::SyncLobbies()
{
}

FString FOnlineSessionFlex::GetFlexConnectionString(FName SessionName)
{
	return FString();
}

void FOnlineSessionFlex::CheckPendingSessionInvite()
{
}

void FOnlineSessionFlex::RegisterLocalPlayers(FNamedOnlineSession* Session)
{
}

FString FOnlineSessionFlex::GetCustomDedicatedServerName() const
{
	return FString();
}

TSharedPtr<const FUniqueNetId> FOnlineSessionFlex::CreateSessionIdFromString(const FString& SessionIdStr)
{
	return TSharedPtr<const FUniqueNetId>();
}


bool FOnlineSessionFlex::CreateSession(int32 HostingPlayerNum, FName SessionName, const FOnlineSessionSettings& NewSessionSettings)
{
	uint32 Result = ONLINE_FAIL;

	FNamedOnlineSession* Session = GetNamedSession(SessionName);

	if (Session == NULL)
	{
		// Create a new session and deep copy the game settings
		Session = AddNamedSession(SessionName, NewSessionSettings);
		check(Session);
		Session->SessionState = EOnlineSessionState::Creating;
		Session->NumOpenPrivateConnections = NewSessionSettings.NumPrivateConnections;
		Session->NumOpenPublicConnections = NewSessionSettings.NumPublicConnections;	// always start with full public connections, local player will register later

		Session->HostingPlayerNum = HostingPlayerNum;

		check(FlexSubsystem);

		// if did not get a valid one, use just something
		if (!Session->OwningUserId.IsValid())
		{
			Session->OwningUserId = MakeShareable(new FUniqueNetIdFlex(FString::Printf(TEXT("%d"), HostingPlayerNum)));
			Session->OwningUserName = FString(TEXT("FlexUser"));
		}

		// Unique identifier of this build for compatibility
		Session->SessionSettings.BuildUniqueId = GetBuildUniqueId();

		// Setup the host session info
		FOnlineSessionInfoFlex* NewSessionInfo = new FOnlineSessionInfoFlex();
		NewSessionInfo->Init(*FlexSubsystem);
		Session->SessionInfo = MakeShareable(NewSessionInfo);

		Result = UpdateMasterServer(Session);

		if (Result != ONLINE_IO_PENDING)
		{
			// Set the game state as pending (not started)
			Session->SessionState = EOnlineSessionState::Pending;

			if (Result != ONLINE_SUCCESS)
			{
				// Clean up the session info so we don't get into a confused state
				RemoveNamedSession(SessionName);
			}
			else
			{
				RegisterLocalPlayers(Session);
			}
		}
	}
	else
	{
		UE_LOG_ONLINE_SESSION(Warning, TEXT("Cannot create session '%s': session already exists."), *SessionName.ToString());
	}

	if (Result != ONLINE_IO_PENDING)
	{
		TriggerOnCreateSessionCompleteDelegates(SessionName, (Result == ONLINE_SUCCESS) ? true : false);
	}

	return Result == ONLINE_IO_PENDING || Result == ONLINE_SUCCESS;
}

uint32 FOnlineSessionFlex::UpdateMasterServer(FNamedOnlineSession* Session) 
{

	return ONLINE_IO_PENDING;


}

bool FOnlineSessionFlex::CreateSession(const FUniqueNetId& HostingPlayerId, FName SessionName, const FOnlineSessionSettings& NewSessionSettings)
{
	return CreateSession(0, SessionName, NewSessionSettings);
}

bool FOnlineSessionFlex::StartSession(FName SessionName)
{
	return false;
}

bool FOnlineSessionFlex::UpdateSession(FName SessionName, FOnlineSessionSettings& UpdatedSessionSettings, bool bShouldRefreshOnlineData)
{
	return false;
}

bool FOnlineSessionFlex::EndSession(FName SessionName)
{
	return false;
}

bool FOnlineSessionFlex::DestroySession(FName SessionName, const FOnDestroySessionCompleteDelegate& CompletionDelegate)
{
	return false;
}

bool FOnlineSessionFlex::IsPlayerInSession(FName SessionName, const FUniqueNetId& UniqueId)
{
	return false;
}

bool FOnlineSessionFlex::StartMatchmaking(const TArray<TSharedRef<const FUniqueNetId>>& LocalPlayers, FName SessionName, const FOnlineSessionSettings& NewSessionSettings, TSharedRef<FOnlineSessionSearch>& SearchSettings)
{
	return false;
}

bool FOnlineSessionFlex::CancelMatchmaking(int32 SearchingPlayerNum, FName SessionName)
{
	return false;
}

bool FOnlineSessionFlex::CancelMatchmaking(const FUniqueNetId& SearchingPlayerId, FName SessionName)
{
	return false;
}

bool FOnlineSessionFlex::FindSessions(int32 SearchingPlayerNum, const TSharedRef<FOnlineSessionSearch>& SearchSettings)
{
	return false;
}

bool FOnlineSessionFlex::FindSessions(const FUniqueNetId& SearchingPlayerId, const TSharedRef<FOnlineSessionSearch>& SearchSettings)
{
	return false;
}

bool FOnlineSessionFlex::FindSessionById(const FUniqueNetId& SearchingUserId, const FUniqueNetId& SessionId, const FUniqueNetId& FriendId, const FOnSingleSessionResultCompleteDelegate& CompletionDelegate)
{
	return false;
}

bool FOnlineSessionFlex::CancelFindSessions()
{
	return false;
}

bool FOnlineSessionFlex::PingSearchResults(const FOnlineSessionSearchResult& SearchResult)
{
	return false;
}

bool FOnlineSessionFlex::JoinSession(int32 PlayerNum, FName SessionName, const FOnlineSessionSearchResult& DesiredSession)
{
	return false;
}

bool FOnlineSessionFlex::JoinSession(const FUniqueNetId& PlayerId, FName SessionName, const FOnlineSessionSearchResult& DesiredSession)
{
	return false;
}

bool FOnlineSessionFlex::FindFriendSession(int32 LocalUserNum, const FUniqueNetId& Friend)
{
	return false;
}

bool FOnlineSessionFlex::FindFriendSession(const FUniqueNetId& LocalUserId, const FUniqueNetId& Friend)
{
	return false;
}

bool FOnlineSessionFlex::FindFriendSession(const FUniqueNetId& LocalUserId, const TArray<TSharedRef<const FUniqueNetId>>& FriendList)
{
	return false;
}

bool FOnlineSessionFlex::SendSessionInviteToFriend(int32 LocalUserNum, FName SessionName, const FUniqueNetId& Friend)
{
	return false;
}

bool FOnlineSessionFlex::SendSessionInviteToFriend(const FUniqueNetId& LocalUserId, FName SessionName, const FUniqueNetId& Friend)
{
	return false;
}

bool FOnlineSessionFlex::SendSessionInviteToFriends(int32 LocalUserNum, FName SessionName, const TArray<TSharedRef<const FUniqueNetId>>& Friends)
{
	return false;
}

bool FOnlineSessionFlex::SendSessionInviteToFriends(const FUniqueNetId& LocalUserId, FName SessionName, const TArray<TSharedRef<const FUniqueNetId>>& Friends)
{
	return false;
}

bool FOnlineSessionFlex::GetResolvedConnectString(FName SessionName, FString& ConnectInfo, FName PortType)
{
	return false;
}

bool FOnlineSessionFlex::GetResolvedConnectString(const FOnlineSessionSearchResult& SearchResult, FName PortType, FString& ConnectInfo)
{
	return false;
}

FOnlineSessionSettings* FOnlineSessionFlex::GetSessionSettings(FName SessionName)
{
	return nullptr;
}

bool FOnlineSessionFlex::RegisterPlayer(FName SessionName, const FUniqueNetId& PlayerId, bool bWasInvited)
{
	return false;
}

bool FOnlineSessionFlex::RegisterPlayers(FName SessionName, const TArray<TSharedRef<const FUniqueNetId>>& Players, bool bWasInvited)
{
	return false;
}

bool FOnlineSessionFlex::UnregisterPlayer(FName SessionName, const FUniqueNetId& PlayerId)
{
	return false;
}

bool FOnlineSessionFlex::UnregisterPlayers(FName SessionName, const TArray<TSharedRef<const FUniqueNetId>>& Players)
{
	return false;
}

void FOnlineSessionFlex::RegisterLocalPlayer(const FUniqueNetId& PlayerId, FName SessionName, const FOnRegisterLocalPlayerCompleteDelegate& Delegate)
{
}

void FOnlineSessionFlex::UnregisterLocalPlayer(const FUniqueNetId& PlayerId, FName SessionName, const FOnUnregisterLocalPlayerCompleteDelegate& Delegate)
{
}

int32 FOnlineSessionFlex::GetNumSessions()
{
	return int32();
}

void FOnlineSessionFlex::DumpSessionState()
{
}
