#if defined _WIN32 || defined _WIN64
#define ONLINESUBSYSTEMFLEXLIBRARY_IMPORT __declspec(dllimport)
#elif defined __linux__
#define ONLINESUBSYSTEMFLEXLIBRARY_IMPORT __attribute__((visibility("default")))
#else
#define ONLINESUBSYSTEMFLEXLIBRARY_IMPORT
#endif

ONLINESUBSYSTEMFLEXLIBRARY_IMPORT void OnlineSubsystemFlexLibraryFunction();
