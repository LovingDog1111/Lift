#include <Windows.h>
#include "Logs/logger.h"
#include "Lift/Lift.h"
#include "HookFactory/HooksFactory.h"
#include "Lift/Features/FeatureFactory.h"
#include "Lift/Commands/CommandFactory.h"
#include <thread>
#include "Values/Client.h"

DWORD WINAPI initClient(LPVOID lpParam) {
    HMODULE hModule = (HMODULE)lpParam;

    Client version;
    if (!version.isUpToDate()) {
        MessageBoxA(
            nullptr,
            "Client version mismatch.\nPlease update the client.",
            "Version Error",
            MB_ICONERROR | MB_OK
        );
        FreeLibraryAndExitThread(hModule, 0);
    }

    Logger::Init();
    Logger::Log("DLL injected!~ meow meow!!");

    CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    Lift::init();
    Lift::checkSigs();
    Lift::checkPads();
    Lift::checkOffsets();
    FeatureFactory::init();
    CommandFactory::init();
    HooksFactory::init();
    if (Game::clientInstance) {
        FeatureFactory::getFeature<ClickGUI>()->InitClickGUI(); //initialize after client instance!
    }
    while (true) {
        Sleep(25);
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        CreateThread(nullptr, 0, initClient, hModule, 0, nullptr);
    }
    else if (ul_reason_for_call == DLL_PROCESS_DETACH) {
        HooksFactory::shutdown();
    }
    return TRUE;
}
