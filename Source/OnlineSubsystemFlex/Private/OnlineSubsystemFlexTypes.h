#include "CoreMinimal.h"
#include "UObject/CoreOnline.h"
#include "OnlineSubsystemTypes.h"

#include "IPAddress.h"


namespace EFlexSession
{
	enum Type
	{
		None,
		LobbySession,
		AdvertisedSessionHost,
		AdvertisedSessionClient,
		LANSession
	};

	inline const TCHAR* ToString(EFlexSession::Type SessionType)
	{
		switch (SessionType)
		{
		case None:
		{
			return TEXT("Session undefined");
		}
		case LobbySession:
		{
			return TEXT("Lobby session");
		}
		case AdvertisedSessionHost:
		{
			return TEXT("Advertised Session Host");
		}
		case AdvertisedSessionClient:
		{
			return TEXT("Advertised Session Client");
		}
		case LANSession:
		{
			return TEXT("LAN Session");
		}
		}
		return TEXT("");
	}
}

class FUniqueNetIdFlex : public FUniqueNetId
{

PACKAGE_SCOPE:
	uint64 UniqueNetId;

	FUniqueNetIdFlex():
		UniqueNetId(0)
	{
			
	}
	explicit FUniqueNetIdFlex(const FUniqueNetIdFlex& Src) :
		UniqueNetId(Src.UniqueNetId)
	{
	}

public:

	explicit FUniqueNetIdFlex(uint64 InUniqueNetId) :
		UniqueNetId(InUniqueNetId)
	{
	}

	explicit FUniqueNetIdFlex(const FString& Str) :
		UniqueNetId(FCString::Atoi64(*Str))
	{
	}

	const uint8* GetBytes() const override
	{
		throw std::logic_error("The method or operation is not implemented.");
	}


	int32 GetSize() const override
	{
		throw std::logic_error("The method or operation is not implemented.");
	}


	bool IsValid() const override
	{
		throw std::logic_error("The method or operation is not implemented.");
	}


	FString ToString() const override
	{
		throw std::logic_error("The method or operation is not implemented.");
	}


	FString ToDebugString() const override
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

};

enum class FFlexConnectionMethod : int8
{
	None = 0,
	Direct,
	P2P,
	PartnerHosted
};


class FOnlineSessionInfoFlex : public FOnlineSessionInfo
{
protected:
	FOnlineSessionInfoFlex(const FOnlineSessionInfoFlex& Src)
	{
	}

	FOnlineSessionInfoFlex& operator=(const FOnlineSessionInfoFlex& Src)
	{
		return *this;
	}

PACKAGE_SCOPE:

	FOnlineSessionInfoFlex(EFlexSession::Type SessionType = EFlexSession::None);

	FOnlineSessionInfoFlex(EFlexSession::Type SessionType, const FUniqueNetIdFlex& InSessionId);

	void Init();

	void InitLAN();

	EFlexSession::Type SessionType;

	TSharedPtr<class FInternetAddr> HostAddr;

	TSharedPtr<class FInternetAddr> FlexP2PAddr;

	FUniqueNetIdFlex SessionId;

	FFlexConnectionMethod ConnectionMethod;

public:

	virtual ~FOnlineSessionInfoFlex()
	{}

	bool operator ==(const FOnlineSessionInfoFlex& Other) const
	{
		return false;
	}

	virtual const uint8* GetBytes() const override
	{
		return nullptr;
	}

	virtual int32 GetSize() const override
	{
		return sizeof(uint64) +
			sizeof(EFlexSession::Type) +
			sizeof(TSharedPtr<class FInternetAddr>) +
			sizeof(TSharedPtr<class FInternetAddr>) +
			sizeof(FUniqueNetIdFlex) +
			sizeof(FFlexConnectionMethod);
	}

	virtual bool IsValid() const override
	{
		switch (SessionType)
		{
		case EFlexSession::LobbySession:
			return FlexP2PAddr.IsValid() && FlexP2PAddr->IsValid() && SessionId.IsValid();
			break;
		case EFlexSession::AdvertisedSessionHost:
		case EFlexSession::AdvertisedSessionClient:
			return ((FlexP2PAddr.IsValid() && FlexP2PAddr->IsValid()) || (HostAddr.IsValid() && HostAddr->IsValid())) && SessionId.IsValid();

			break;
		case EFlexSession::LANSession:
		default:
			return HostAddr.IsValid()&& HostAddr.IsValid();
		}
	}

	virtual FString ToString() const override
	{
		return SessionId.ToString();
	}

	virtual	FString ToDebugString() const override
	{
		return FString::Printf(TEXT("HostIP: %s FlexP2P: %s Type: %s SessionId: %s"),
			HostAddr.IsValid() ? *HostAddr->ToString(true) : TEXT("INVALID"),
			FlexP2PAddr.IsValid() ? *FlexP2PAddr->ToString(true) : TEXT("INVALID"),
			EFlexSession::ToString(SessionType), *SessionId.ToDebugString());
	}

	virtual const FUniqueNetId& GetSessionId() const override
	{
		return SessionId;
	}












};