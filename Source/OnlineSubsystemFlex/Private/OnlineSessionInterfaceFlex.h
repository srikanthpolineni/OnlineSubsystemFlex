#pragma once
#include "CoreMinimal.h"
#include "UObject/CoreOnline.h"
#include "OnlineSubsystemFlexTypes.h"
#include "Misc/ScopeLock.h"
#include "OnlineKeyValuePair.h"
#include "OnlineSessionSettings.h"
#include "OnlineSessionInterface.h"


class FOnlineSessionFlex : public IOnlineSession
{
protected:
	class FOnlineSubsystemFlex* FlexSubsystem;

	class FLANSession* LANSession;

	FOnlineSessionFlex() :
		FlexSubsystem(NULL),
		LANSession(NULL),
		bFlexibleGameServerConnected(false),
		GameServerId(NULL),
		CurrentSessionSearch(NULL)
	{}

	void TickLanTasks(float DeltaTime);

	void TickPendingInvites(float DeltaTime);

	uint32 CreateLobbySession(int32 HostingPlayerNum, class FNamedOnlineSession* Session);

	uint32 CreateInternetSession(int32 HostingPlayerNum, class FNamedOnlineSession* Session);

	uint32 JoinLobbySession(int32 PlayerNumber, class FNamedOnlineSession* Session, const FOnlineSession* SearchSession);

	uint32 JoinInternetSession(int32 PlayerNumber, class FNamedOnlineSession* Session, const FOnlineSession* SearchSession);

	uint32 EndInternetSession(class FNamedOnlineSession* Session);

	uint32 DestroyLobbySession(class FNamedOnlineSession* Session, const FOnDestroySessionCompleteDelegate& CompletionDelegate);

	uint32 DestroyInternetSession(class FNamedOnlineSession* Session, const FOnDestroySessionCompleteDelegate& CompletionDelegate);

	uint32 CreateLANSession(int32 HostingPlayerNum, class FNamedOnlineSession* Session);

	uint32 JoinLANSession(int32 PlayerNum, class FNamedOnlineSession* Session, const class FOnlineSession* SearchSession);

	uint32 FindInternetSession(const TSharedRef<FOnlineSessionSearch>& SearchSettings);

	uint32 FindLANSession(const TSharedRef<FOnlineSessionSearch>& SearchSettings);

	void AppendSessionToPacket(class FNboSerializeToBufferSteam& Packet, class FOnlineSession* Session);

	void AppendSessionSettingsToPacket(class FNboSerializeToBufferSteam& Packet, FOnlineSessionSettings* SessionSettings);

	void ReadSessionFromPacket(class FNboSerializeFromBufferSteam& Packet, class FOnlineSession* Session);

	void ReadSettingsFromPacket(class FNboSerializeFromBufferSteam& Packet, FOnlineSessionSettings& SessionSettings);

	void OnValidQueryPacketReceived(uint8* PacketData, int32 PacketLength, uint64 ClientNonce);

	void OnValidResponsePacketReceived(uint8* PacketData, int32 PacketLength);

	void OnLANSearchTimeout();

	void RegisterVoice(const FUniqueNetId& PlayerId);

	void UnregisterVoice(const FUniqueNetId& PlayerId);

	uint32 CreateSessionOnMasterServer(FNamedOnlineSession* Session);


PACKAGE_SCOPE:
	mutable FCriticalSection SessionLock;

	TArray<FNamedOnlineSession> Sessions;

	bool bFlexibleGameServerConnected;

	TSharedPtr<FUniqueNetIdFlex> GameServerId;

	TSharedPtr<FOnlineSessionSearch> CurrentSessionSearch;

	FCriticalSection JoinedLobbyLock;

	TArray<FUniqueNetIdFlex> JoinedLobbyList;

	FOnlineSessionFlex(class FOnlineSubsystemFlex* InSubSystem) :
		FlexSubsystem(InSubSystem),
		LANSession(NULL),
		bFlexibleGameServerConnected(false),
		GameServerId(NULL),
		CurrentSessionSearch(NULL)
	{}

	void Tick(float DeltaTime);

	class FNamedOnlineSession* AddNamedSession(FName SessionName, const FOnlineSessionSettings& SessionSettings) override
	{
		FScopeLock ScopeLock(&SessionLock);
		return new (Sessions) FNamedOnlineSession(SessionName, SessionSettings);
	}

	class FNamedOnlineSession* AddNamedSession(FName SessionName, const FOnlineSession& Session) override
	{
		FScopeLock ScopeLock(&SessionLock);
		return new (Sessions) FNamedOnlineSession(SessionName, Session);
	}

	inline FNamedOnlineSession* GetNamedSessionFromLobbyId(FUniqueNetIdFlex& LobbyId)
	{
		FScopeLock ScopeLock(&SessionLock);
		for (int32 SearchIndex = 0; SearchIndex < Sessions.Num(); SearchIndex++)
		{
			FNamedOnlineSession& Session = Sessions[SearchIndex];
			if (Session.SessionInfo.IsValid())
			{
				FOnlineSessionInfoFlex* SessionInfo = (FOnlineSessionInfoFlex*)Session.SessionInfo.Get();
				if (SessionInfo->SessionType == EFlexSession::LANSession && SessionInfo->SessionId == LobbyId)
				{
					return &Sessions[SearchIndex];
				}
			}
		}
		return NULL;
	}

	inline FNamedOnlineSession* GetGameServerSession()
	{
		FScopeLock ScopeLock(&SessionLock);
		for (int32 SearchIndex = 0; SearchIndex < Sessions.Num(); SearchIndex++)
		{
			FNamedOnlineSession& Session = Sessions[SearchIndex];
			if (Session.SessionInfo.IsValid())
			{
				FOnlineSessionInfoFlex* SessionInfo = (FOnlineSessionInfoFlex*)Session.SessionInfo.Get();
				if (SessionInfo->SessionType == EFlexSession::LANSession)
				{
					return &Sessions[SearchIndex];
				}
			}
		}
		return NULL;
	}

	void SyncLobbies();

	void JoinedLobby(FUniqueNetIdFlex& LobbyId)
	{
		FScopeLock ScopeLock(&JoinedLobbyLock);
		JoinedLobbyList.Add(LobbyId);
	}

	void LeftLobby(FUniqueNetIdFlex LobbyId)
	{
		FScopeLock ScopeLock(&JoinedLobbyLock);
		JoinedLobbyList.RemoveSingleSwap(LobbyId);
	}

	bool IsMemberOfLobby(FUniqueNetIdFlex& LobbyId)
	{
		FScopeLock ScopeLock(&JoinedLobbyLock);
		return JoinedLobbyList.Find(LobbyId) != INDEX_NONE;
	}


	FString GetFlexConnectionString(FName SessionName);

	void CheckPendingSessionInvite();

	void RegisterLocalPlayers(class FNamedOnlineSession* Session);

	FString GetCustomDedicatedServerName() const;

	void OnCreateSessionComplete(FName SessionName, const FOnlineError& Error);

public:
	
	virtual ~FOnlineSessionFlex() {}

	virtual TSharedPtr<const FUniqueNetId> CreateSessionIdFromString(const FString& SessionIdStr) override;

	FNamedOnlineSession* GetNamedSession(FName SessionName) override
	{
		FScopeLock ScopeLock(&SessionLock);
		for (int32 SearchIndex = 0; SearchIndex < Sessions.Num(); SearchIndex++)
		{
			if (Sessions[SearchIndex].SessionName == SessionName)
			{
				return &Sessions[SearchIndex];
			}
		}
		return NULL;
	}

	virtual void RemoveNamedSession(FName SessionName) override
	{
		FScopeLock ScopeLock(&SessionLock);
		for (int32 SearchIndex = 0; SearchIndex < Sessions.Num(); SearchIndex++)
		{
			if (Sessions[SearchIndex].SessionName == SessionName)
			{
				Sessions.RemoveAtSwap(SearchIndex);
				return;
			}
		}
	}

	virtual EOnlineSessionState::Type GetSessionState(FName SessionName) const
	{
		FScopeLock ScopeLock(&SessionLock);
		for (int32 SearchIndex = 0; SearchIndex < Sessions.Num(); SearchIndex++)
		{
			if (Sessions[SearchIndex].SessionName == SessionName)
			{
				return Sessions[SearchIndex].SessionState;
			}
		}
		return EOnlineSessionState::NoSession;
	}

	virtual bool HasPresenceSession() override
	{
		FScopeLock ScopeLock(&SessionLock);
		for (int32 SearchIndex = 0; SearchIndex < Sessions.Num(); SearchIndex++)
		{
			if (Sessions[SearchIndex].SessionSettings.bUsesPresence)
			{
				return true;
			}
		}
		return false;
	}

	// IOnlineSession
	virtual bool CreateSession(int32 HostingPlayerNum, FName SessionName, const FOnlineSessionSettings& NewSessionSettings) override;
	virtual bool CreateSession(const FUniqueNetId& HostingPlayerId, FName SessionName, const FOnlineSessionSettings& NewSessionSettings) override;
	virtual bool StartSession(FName SessionName) override;
	virtual bool UpdateSession(FName SessionName, FOnlineSessionSettings& UpdatedSessionSettings, bool bShouldRefreshOnlineData = true) override;
	virtual bool EndSession(FName SessionName) override;
	virtual bool DestroySession(FName SessionName, const FOnDestroySessionCompleteDelegate& CompletionDelegate = FOnDestroySessionCompleteDelegate()) override;
	virtual bool IsPlayerInSession(FName SessionName, const FUniqueNetId& UniqueId) override;
	virtual bool StartMatchmaking(const TArray< TSharedRef<const FUniqueNetId> >& LocalPlayers, FName SessionName, const FOnlineSessionSettings& NewSessionSettings, TSharedRef<FOnlineSessionSearch>& SearchSettings) override;
	virtual bool CancelMatchmaking(int32 SearchingPlayerNum, FName SessionName) override;
	virtual bool CancelMatchmaking(const FUniqueNetId& SearchingPlayerId, FName SessionName) override;
	virtual bool FindSessions(int32 SearchingPlayerNum, const TSharedRef<FOnlineSessionSearch>& SearchSettings) override;
	virtual bool FindSessions(const FUniqueNetId& SearchingPlayerId, const TSharedRef<FOnlineSessionSearch>& SearchSettings) override;
	virtual bool FindSessionById(const FUniqueNetId& SearchingUserId, const FUniqueNetId& SessionId, const FUniqueNetId& FriendId, const FOnSingleSessionResultCompleteDelegate& CompletionDelegate) override;
	virtual bool CancelFindSessions() override;
	virtual bool PingSearchResults(const FOnlineSessionSearchResult& SearchResult) override;
	virtual bool JoinSession(int32 PlayerNum, FName SessionName, const FOnlineSessionSearchResult& DesiredSession) override;
	virtual bool JoinSession(const FUniqueNetId& PlayerId, FName SessionName, const FOnlineSessionSearchResult& DesiredSession) override;
	virtual bool FindFriendSession(int32 LocalUserNum, const FUniqueNetId& Friend) override;
	virtual bool FindFriendSession(const FUniqueNetId& LocalUserId, const FUniqueNetId& Friend) override;
	virtual bool FindFriendSession(const FUniqueNetId& LocalUserId, const TArray<TSharedRef<const FUniqueNetId>>& FriendList) override;
	virtual bool SendSessionInviteToFriend(int32 LocalUserNum, FName SessionName, const FUniqueNetId& Friend) override;
	virtual bool SendSessionInviteToFriend(const FUniqueNetId& LocalUserId, FName SessionName, const FUniqueNetId& Friend) override;
	virtual bool SendSessionInviteToFriends(int32 LocalUserNum, FName SessionName, const TArray< TSharedRef<const FUniqueNetId> >& Friends) override;
	virtual bool SendSessionInviteToFriends(const FUniqueNetId& LocalUserId, FName SessionName, const TArray< TSharedRef<const FUniqueNetId> >& Friends) override;
	virtual bool GetResolvedConnectString(FName SessionName, FString& ConnectInfo, FName PortType) override;
	virtual bool GetResolvedConnectString(const FOnlineSessionSearchResult& SearchResult, FName PortType, FString& ConnectInfo) override;
	virtual FOnlineSessionSettings* GetSessionSettings(FName SessionName) override;
	virtual bool RegisterPlayer(FName SessionName, const FUniqueNetId& PlayerId, bool bWasInvited) override;
	virtual bool RegisterPlayers(FName SessionName, const TArray< TSharedRef<const FUniqueNetId> >& Players, bool bWasInvited = false) override;
	virtual bool UnregisterPlayer(FName SessionName, const FUniqueNetId& PlayerId) override;
	virtual bool UnregisterPlayers(FName SessionName, const TArray< TSharedRef<const FUniqueNetId> >& Players) override;
	virtual void RegisterLocalPlayer(const FUniqueNetId& PlayerId, FName SessionName, const FOnRegisterLocalPlayerCompleteDelegate& Delegate) override;
	virtual void UnregisterLocalPlayer(const FUniqueNetId& PlayerId, FName SessionName, const FOnUnregisterLocalPlayerCompleteDelegate& Delegate) override;
	virtual int32 GetNumSessions() override;
	virtual void DumpSessionState() override;

};

typedef TSharedPtr<FOnlineSessionFlex, ESPMode::ThreadSafe> FOnlineSessionFlexPtr;