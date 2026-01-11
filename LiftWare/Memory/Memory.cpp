#include "Memory.h"
#include "../Logs/Logger.h"
#include <libhat/Scanner.hpp>

uintptr_t Memory::GetGameBase() {
    static uintptr_t base = reinterpret_cast<uintptr_t>(GetModuleHandleA("Minecraft.Windows.exe"));
    return base;
}

uintptr_t findSignatureSolsticeThief (std::string_view sig) {
    auto address = hat::parse_signature(sig);
    if (!address.has_value()) {
        return 0;
    }
    auto signature = address.value();

    const auto module = hat::process::get_process_module();
    auto result = hat::find_pattern(signature, ".text", module);

    if (result.has_result())
        return reinterpret_cast<uintptr_t>(result.get());

    return 0;
}

uintptr_t** Memory::getVtableFromSigSolstice(std::string_view sig) {
    uintptr_t address = findSignatureSolsticeThief(sig);

    if (address == 0x0)
        return nullptr;

    int finalOffset = *reinterpret_cast<int*>((address + 3));
    return reinterpret_cast<uintptr_t**>(address + finalOffset + 7);
}

uintptr_t Memory::FindSignature(std::string_view sig, std::string_view name) {
    auto parsed = hat::parse_signature(sig);
    if (!parsed.has_value()) {
        if (!name.empty()) {
            std::string msg = "Signature parse failed: " + std::string(name);
            Logger::ErrorLog(msg);
            Logger::LogToFile(msg);
        }
        else {
            std::string msg = "Signature parse failed";
            Logger::ErrorLog(msg);
            Logger::LogToFile(msg);
        }
        return 0;
    }

    auto result = hat::find_pattern(parsed.value());
    if (!result.has_result()) {
        if (!name.empty()) {
            std::string msg = "Signature not found: " + std::string(name);
            Logger::ErrorLog(msg);
            Logger::LogToFile(msg);
        }
        else {
            std::string msg = "Signature not found";
            Logger::ErrorLog(msg);
            Logger::LogToFile(msg);
        }
        return 0;
    }

    uintptr_t addr = reinterpret_cast<uintptr_t>(result.get());
    if (!name.empty()) {
        std::string msg = "Signature found: " + std::string(name);
        Logger::GoodLog(msg);
        Logger::LogToFile(msg);
    }
    else {
        std::string msg = "Signature found";
        Logger::GoodLog(msg);
        Logger::LogToFile(msg);
    }

    return addr;
}

uintptr_t** Memory::GetVTableFromSignature(std::string_view sig) {
    uintptr_t address = FindSignature(sig);

    if (address == 0x0)
        return nullptr;

    int finalOffset = *reinterpret_cast<int*>((address + 3));
    return reinterpret_cast<uintptr_t**>(address + finalOffset + 7);
}
