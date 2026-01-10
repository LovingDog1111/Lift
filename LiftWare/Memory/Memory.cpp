#include "Memory.h"
#include "../Logs/Logger.h"
#include <libhat/Scanner.hpp>

uintptr_t Memory::GetGameBase() {
    static uintptr_t base = reinterpret_cast<uintptr_t>(GetModuleHandleA("Minecraft.Windows.exe"));
    return base;
}

uintptr_t Memory::FindSignature(std::string_view sig, std::string_view name) {
    auto parsed = hat::parse_signature(sig);
    if (!parsed.has_value()) {
        if (!name.empty())
            Logger::ErrorLog("Signature parse failed: " + std::string(name));
        else
            Logger::ErrorLog("Signature parse failed");
        return 0;
    }

    auto result = hat::find_pattern(parsed.value());
    if (!result.has_result()) {
        if (!name.empty())
            Logger::ErrorLog("Signature not found: " + std::string(name));
        else
            Logger::ErrorLog("Signature not found");
        return 0;
    }

    uintptr_t addr = reinterpret_cast<uintptr_t>(result.get());
    if (!name.empty())
        Logger::GoodLog("Signature found: " + std::string(name));
    else
        Logger::GoodLog("Signature found");

    return addr;
}

uintptr_t** Memory::GetVTableFromSignature(std::string_view sig) {
    uintptr_t a = FindSignature(sig);
    if (!a)
        return nullptr;

    int o = *reinterpret_cast<int*>(a + 3);
    return reinterpret_cast<uintptr_t**>(a + o + 7);
}
