#pragma once


typedef TSharedPtr<class FFlexMasterServer, ESPMode::ThreadSafe> FFlexMasterServerPtr;
class FFlexMasterServer
{

public:
	FFlexMasterServer(FFlexMasterServer const&) = delete;
	FFlexMasterServer& operator=(FFlexMasterServer const&) = delete;

	static FFlexMasterServerPtr Instance()
	{
		static FFlexMasterServerPtr  ins{ MakeShareable(new FFlexMasterServer) };
		return ins;
	}

private:
	FFlexMasterServer() {}

};

