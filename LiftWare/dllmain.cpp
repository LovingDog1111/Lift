#include <Windows.h>
#include "Logs/logger.h"
#include "MainClient/Lift.h"
#include "HookFactory/HooksFactory.h"
#include "MainClient/Features/FeatureFactory.h"
#include "MainClient/Commands/CommandFactory.h"

DWORD WINAPI initClient(LPVOID) {
    Logger::Init();
    Logger::Log("DLL injected");

    CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    Lift::init();
    Lift::checkSigs();
    FeatureFactory::init();
    CommandFactory::init();
    HooksFactory::init(); //init AFTER THE CLIENT

    while (true) {
        Sleep(25);
    }

    HooksFactory::shutdown();
    FeatureFactory::shutdown();
    Lift::shutdown();
    CoUninitialize();
    Logger::Shutdown();
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)initClient, hModule, 0, 0);
        break;
    case DLL_PROCESS_DETACH:
        HooksFactory::shutdown();
        break;
    }
    return TRUE;
}

