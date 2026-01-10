#include <Windows.h>
#include "Logs/logger.h"
#include "MainClient/Lift.h"

DWORD WINAPI initClient(LPVOID) {
    Logger::Init();
    Logger::Log("DLL injected");

    CoInitializeEx(nullptr, COINIT_MULTITHREADED);

    Lift::init();
    Lift::checkSigs();

    while (true) {
        Sleep(25);
    }

    Lift::shutdown();
    CoUninitialize();
    Logger::Shutdown();
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        CreateThread(nullptr, 0, initClient, nullptr, 0, nullptr);
    }
    return TRUE;
}
