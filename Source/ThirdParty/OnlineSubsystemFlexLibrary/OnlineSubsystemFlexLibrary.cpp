#if defined _WIN32 || defined _WIN64
    #include <Windows.h>

    #define ONLINESUBSYSTEMFLEXLIBRARY_EXPORT __declspec(dllexport)
#else
    #include <stdio.h>
#endif

#ifndef ONLINESUBSYSTEMFLEXLIBRARY_EXPORT
    #define ONLINESUBSYSTEMFLEXLIBRARY_EXPORT
#endif

ONLINESUBSYSTEMFLEXLIBRARY_EXPORT void OnlineSubsystemFlexLibraryFunction()
{
#if defined _WIN32 || defined _WIN64
	MessageBox(NULL, TEXT("Loaded OnlineSubsystemFlexLibrary.dll from Third Party Plugin sample."), TEXT("Third Party Plugin"), MB_OK);
#else
    printf("Loaded OnlineSubsystemFlexLibrary from Third Party Plugin sample");
#endif
}