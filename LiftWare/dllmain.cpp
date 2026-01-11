#include <Windows.h>
#include <thread>
#include "Logs/logger.h"
#include "Lift/Lift.h"
#include "HookFactory/HooksFactory.h"
#include "Lift/Features/FeatureFactory.h"
#include "Lift/Commands/CommandFactory.h"
#include "Values/Client.h"

static HANDLE gThread = nullptr;
static bool gRunning = true;

DWORD WINAPI initClient(LPVOID lpParam) {
    CoInitializeEx(nullptr, COINIT_MULTITHREADED);

    Logger::Init();
    Logger::Log("DLL injected!~ meow meow!!");

    Lift::init();
    Lift::checkSigs();
    Lift::checkPads();
    Lift::checkOffsets();

    FeatureFactory::init();
    CommandFactory::init();
    HooksFactory::init();

    while (gRunning) {
        Sleep(25);
    }

    HooksFactory::shutdown();
    CoUninitialize();
    FreeLibraryAndExitThread((HMODULE)lpParam, 0);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        gThread = CreateThread(nullptr, 0, initClient, hModule, 0, nullptr);
    }
    else if (reason == DLL_PROCESS_DETACH) {
        gRunning = false;
        if (gThread) {
            WaitForSingleObject(gThread, INFINITE);
            CloseHandle(gThread);
        }
    }
    return TRUE;
}
