#include <Windows.h>
#include <string>
#include <memory>
#include <atomic>
#include <unordered_set>
#include <mutex>
#include <TlHelp32.h>
#include "Logs/logger.h"
#include "Lift/Lift.h"
#include "HookFactory/HooksFactory.h"
#include "Lift/Features/FeatureFactory.h"
#include "Lift/Commands/CommandFactory.h"
#include "Values/Client.h"

class GlobalState {
public:
    static GlobalState& Instance() {
        static GlobalState instance;
        return instance;
    }

    void Initialize(HMODULE module) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (initialized_) return;

        hModule_ = module;
        running_ = true;
        threadHandle_ = nullptr;
        initialized_ = true;
    }

    void Shutdown() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!initialized_) return;

        running_ = false;

        if (threadHandle_) {
            WaitForSingleObject(threadHandle_, 2000);
            CloseHandle(threadHandle_);
            threadHandle_ = nullptr;
        }

        initialized_ = false;
    }

    bool IsRunning() const { return running_; }
    HMODULE GetModule() const { return hModule_; }
    void SetThreadHandle(HANDLE handle) { threadHandle_ = handle; }

private:
    HMODULE hModule_ = nullptr;
    HANDLE threadHandle_ = nullptr;
    std::atomic<bool> running_{ false };
    std::atomic<bool> initialized_{ false };
    mutable std::mutex mutex_;
};

class WindowManager {
public:
    static void Initialize() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (initialized_) return;

        originalProc_ = nullptr;
        hookProc_ = nullptr;
        targetWindow_ = nullptr;
        initialized_ = true;

        std::thread(&WindowManager::SetupWindowHook).detach();
    }

    static void SetTitle(const std::wstring& title) {
        std::lock_guard<std::mutex> lock(mutex_);
        customTitle_ = title;

        if (targetWindow_) {
            SetWindowTextW(targetWindow_, title.c_str());
        }
    }

    static void ShowNotification(const std::wstring& message) {
        if (targetWindow_) {
            PostMessageW(targetWindow_, WM_USER + 100, 0,
                reinterpret_cast<LPARAM>(new std::wstring(message)));
        }
    }

    static void Cleanup() {
        std::lock_guard<std::mutex> lock(mutex_);

        if (targetWindow_ && originalProc_) {
            SetWindowLongPtrW(targetWindow_, GWLP_WNDPROC,
                reinterpret_cast<LONG_PTR>(originalProc_));
        }

        initialized_ = false;
        targetWindow_ = nullptr;
        originalProc_ = nullptr;
        hookProc_ = nullptr;
    }

private:
    static void SetupWindowHook() {
        for (int i = 0; i < 100 && !targetWindow_; ++i) {
            targetWindow_ = FindWindowW(nullptr, nullptr);
            if (targetWindow_) {
                originalProc_ = reinterpret_cast<WNDPROC>(
                    GetWindowLongPtrW(targetWindow_, GWLP_WNDPROC));
                hookProc_ = WindowHookProc;
                SetWindowLongPtrW(targetWindow_, GWLP_WNDPROC,
                    reinterpret_cast<LONG_PTR>(hookProc_));

                if (!customTitle_.empty()) {
                    SetWindowTextW(targetWindow_, customTitle_.c_str());
                }
                break;
            }
            Sleep(100);
        }
    }

    static LRESULT CALLBACK WindowHookProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        if (msg == WM_USER + 100) {
            auto* notification = reinterpret_cast<std::wstring*>(lParam);
            if (notification) {
                MessageBoxW(hwnd, notification->c_str(), L"Notification", MB_ICONINFORMATION);
                delete notification;
            }
            return 0;
        }

        if (originalProc_) {
            return CallWindowProcW(originalProc_, hwnd, msg, wParam, lParam);
        }

        return DefWindowProcW(hwnd, msg, wParam, lParam);
    }

    static inline HWND targetWindow_;
    static inline WNDPROC originalProc_;
    static inline WNDPROC hookProc_;
    static inline std::wstring customTitle_;
    static inline std::atomic<bool> initialized_{ false };
    static inline std::mutex mutex_;
};

class InjectionGuard {
public:
    static bool IsAlreadyInjected() {
        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());
        if (snapshot == INVALID_HANDLE_VALUE) return false;

        MODULEENTRY32W moduleEntry{};
        moduleEntry.dwSize = sizeof(moduleEntry);

        std::wstring currentModuleName;
        wchar_t currentModulePath[MAX_PATH];
        GetModuleFileNameW(GetModuleHandle(nullptr), currentModulePath, MAX_PATH);
        currentModuleName = currentModulePath;

        size_t count = 0;
        for (BOOL success = Module32FirstW(snapshot, &moduleEntry);
            success;
            success = Module32NextW(snapshot, &moduleEntry)) {

            std::wstring moduleName = moduleEntry.szModule;
            if (moduleName.find(L"injected") != std::wstring::npos ||
                moduleName.find(currentModuleName) != std::wstring::npos) {
                count++;
            }
        }

        CloseHandle(snapshot);
        return count > 1;
    }
};

class CleanupManager {
public:
    ~CleanupManager() {
        PerformCleanup();
    }

    void AddCleanupTask(std::function<void()> task) {
        std::lock_guard<std::mutex> lock(mutex_);
        cleanupTasks_.push_back(task);
    }

    void PerformCleanup() {
        std::lock_guard<std::mutex> lock(mutex_);

        for (auto it = cleanupTasks_.rbegin(); it != cleanupTasks_.rend(); ++it) {
            try {
                (*it)();
            }
            catch (...) {
            }
        }

        cleanupTasks_.clear();
    }

private:
    std::vector<std::function<void()>> cleanupTasks_;
    std::mutex mutex_;
};

DWORD WINAPI ClientThread(LPVOID lpParam) {
    HMODULE module = static_cast<HMODULE>(lpParam);
    GlobalState::Instance().Initialize(module);

    std::unique_ptr<CleanupManager> cleanupManager = std::make_unique<CleanupManager>();

    cleanupManager->AddCleanupTask([]() {
        HooksFactory::shutdown();
        });

    cleanupManager->AddCleanupTask([]() {
        WindowManager::Cleanup();
        });

    cleanupManager->AddCleanupTask([]() {
        CoUninitialize();
        });

    HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    if (FAILED(hr)) {
        return 1;
    }

    if (InjectionGuard::IsAlreadyInjected()) {
        FreeLibraryAndExitThread(module, 0);
        return 0;
    }

    Logger::Init();
    Logger::Log("DLL injected successfully");

    WindowManager::Initialize();
    WindowManager::SetTitle(L"Lift Client");
    WindowManager::ShowNotification(L"Injection successful");

    Lift::init();
    Lift::checkSigs();
    Lift::checkPads();
    Lift::checkOffsets();

    FeatureFactory::init();
    CommandFactory::init();
    HooksFactory::init();

    while (GlobalState::Instance().IsRunning()) {
        Sleep(25);

        static int frameCount = 0;
        frameCount++;

        if (frameCount % 200 == 0) {
            WindowManager::ShowNotification(L"Client is running...");
        }
    }

    cleanupManager->PerformCleanup();

    FreeLibraryAndExitThread(module, 0);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID) {
    switch (reason) {
    case DLL_PROCESS_ATTACH: {
        DisableThreadLibraryCalls(hModule);

        if (InjectionGuard::IsAlreadyInjected()) {
            return FALSE;
        }

        HANDLE thread = CreateThread(
            nullptr,
            0,
            ClientThread,
            hModule,
            CREATE_SUSPENDED,
            nullptr
        );

        if (thread) {
            GlobalState::Instance().SetThreadHandle(thread);
            ResumeThread(thread);
            CloseHandle(thread);
        }

        break;
    }

    case DLL_PROCESS_DETACH: {
        GlobalState::Instance().Shutdown();
        WindowManager::Cleanup();
        break;
    }

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        break;
    }

    return TRUE;
}