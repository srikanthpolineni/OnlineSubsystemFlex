#pragma once

#include "CoreMinimal.h"
#include "OnlineDelegateMacros.h"
#include "OnlineSubsystemImpl.h"

#ifndef FLEX_SUBSYSTEM
#define FLEX_SUBSYSTEM FName(TEXT("FLEX"))
#endif


class FOnlineSessionFlex;

typedef TSharedPtr<class FOnlineSessionFlex, ESPMode::ThreadSafe> FOnlineSessionFlexPtr;
typedef TSharedPtr<class FOnlineIdentityFlex, ESPMode::ThreadSafe> FOnlineIdentityFlexPtr;
typedef TSharedPtr<class FOnlineFriendsFlex, ESPMode::ThreadSafe> FOnlineFriendsFlexPtr;

class ONLINESUBSYSTEMFLEX_API FOnlineSubsystemFlex :
	public FOnlineSubsystemImpl
{

protected:

	bool bFlexLibraryClientInitialized;

	bool bFlexLibraryGameServerInitialized;

	/** the local port used to communicate with the servers */
	uint32 GameServerLocalPort;

	/** Game port - the port that clients will connect to for game play */
	uint32 GameServerGamePort;

	uint32 GameServerQueryPort;

	FOnlineSessionFlexPtr SessionInterface;

	FOnlineIdentityFlexPtr IdentityInterface;

	FOnlineFriendsFlexPtr FriendInterface;

	mutable IOnlineVoicePtr VoiceInterface;

	mutable bool bVoiceInterfaceInitialized;



PACKAGE_SCOPE:
	FOnlineSubsystemFlex() = delete;
	FOnlineSubsystemFlex(FName InInstanceName) :
		FOnlineSubsystemImpl(FLEX_SUBSYSTEM, InInstanceName),
		GameServerLocalPort(0),
		GameServerGamePort(0),
		GameServerQueryPort(0),
		SessionInterface(nullptr),
		IdentityInterface(nullptr),
		FriendInterface(nullptr),
		VoiceInterface(nullptr),
		bVoiceInterfaceInitialized(false)


	{}

public:
	virtual ~FOnlineSubsystemFlex()
	{}

	virtual IOnlineSessionPtr GetSessionInterface() const override;
	virtual IOnlineFriendsPtr GetFriendsInterface() const override;
	virtual IOnlinePartyPtr GetPartyInterface() const override;
	virtual IOnlineGroupsPtr GetGroupsInterface() const override;
	virtual IOnlineSharedCloudPtr GetSharedCloudInterface() const override;
	virtual IOnlineUserCloudPtr GetUserCloudInterface() const override;
	virtual IOnlineLeaderboardsPtr GetLeaderboardsInterface() const override;
	virtual IOnlineVoicePtr GetVoiceInterface() const override;
	virtual IOnlineExternalUIPtr GetExternalUIInterface() const override;
	virtual IOnlineTimePtr GetTimeInterface() const override;
	virtual IOnlineIdentityPtr GetIdentityInterface() const override;
	virtual IOnlineTitleFilePtr GetTitleFileInterface() const override;
	virtual IOnlineEntitlementsPtr GetEntitlementsInterface() const override;
	virtual IOnlineStoreV2Ptr GetStoreV2Interface() const override { return nullptr; }
	virtual IOnlinePurchasePtr GetPurchaseInterface() const override { return nullptr; }
	virtual IOnlineEventsPtr GetEventsInterface() const override;
	virtual IOnlineAchievementsPtr GetAchievementsInterface() const override;
	virtual IOnlineSharingPtr GetSharingInterface() const override;
	virtual IOnlineUserPtr GetUserInterface() const override;
	virtual IOnlineMessagePtr GetMessageInterface() const override;
	virtual IOnlinePresencePtr GetPresenceInterface() const override;
	virtual IOnlineChatPtr GetChatInterface() const override;
	virtual IOnlineStatsPtr GetStatsInterface() const override;
	virtual IOnlineTurnBasedPtr GetTurnBasedInterface() const override;
	virtual IOnlineTournamentPtr GetTournamentInterface() const override;
	virtual bool IsLocalPlayer(const FUniqueNetId& UniqueId) const override;
	virtual bool Init() override;
	virtual bool Shutdown() override;
	virtual bool Exec(class UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar) override;
	virtual bool IsEnabled() const override;
	virtual FString GetAppId() const override;
	virtual FText GetOnlineServiceName() const override;


	virtual bool Tick(float DeltaTime) override;
};

typedef TSharedPtr<FOnlineSubsystemFlex, ESPMode::ThreadSafe> FOnlineSubsystemFlexPtr;