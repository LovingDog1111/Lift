#include "NoEatBob.h"

#ifdef _WIN32
#include <windows.h>
#endif

NoEatBob::NoEatBob() : Feature("NoEatBob", "Disables eat-bob animation", Category::VISUAL) {
    tryLocate();
    if (isEnabled()) applyPatch();
}

NoEatBob::~NoEatBob() {
    restorePatch();
}

void NoEatBob::onEnable() {
    tryLocate();
    applyPatch();
}

void NoEatBob::onDisable() {
    restorePatch();
}

void NoEatBob::tryLocate() {
    if (sAddr) return;
    void* p = (void*)Memory::FindSignature(Sigs::NoEatBobSig);
    if (!p) return;

    sAddr = p;
    if (!sSaved) {
        Memory::copyBytes(sAddr, sOg5, 5);
        sSaved = true;
    }
}

void NoEatBob::applyPatch() {
    if (!sAddr || !sSaved) return;

    uint8_t cur[5]{};
    Memory::copyBytes(sAddr, cur, 5);
    if (cur[0] == 0x0F && cur[1] == 0x57) { sPatched = true; return; }
    if (!(cur[0] == 0xF3 && cur[1] == 0x0F && cur[2] == 0x10)) return;

    const uint8_t modrm = cur[3];
    const uint8_t reg = (modrm >> 3) & 0x07;
    const uint8_t rr = (uint8_t)(0xC0 | (reg << 3) | reg);

    uint8_t patch[5] = { 0x0F, 0x57, rr, 0x90, 0x90 };

#ifdef _WIN32
    DWORD oldProt{};
    VirtualProtect(sAddr, 5, PAGE_EXECUTE_READWRITE, &oldProt);
#endif

    Memory::patchBytes(sAddr, patch, 5);

#ifdef _WIN32
    FlushInstructionCache(GetCurrentProcess(), sAddr, 5);
    VirtualProtect(sAddr, 5, oldProt, &oldProt);
#endif

    sPatched = true;
}

void NoEatBob::restorePatch() {
    if (!sAddr || !sSaved) { sPatched = false; return; }

    uint8_t cur[5]{};
    Memory::copyBytes(sAddr, cur, 5);
    if (!(cur[0] == 0x0F && cur[1] == 0x57)) { sPatched = false; return; }

#ifdef _WIN32
    DWORD oldProt{};
    VirtualProtect(sAddr, 5, PAGE_EXECUTE_READWRITE, &oldProt);
#endif

    Memory::patchBytes(sAddr, sOg5, 5);

#ifdef _WIN32
    FlushInstructionCache(GetCurrentProcess(), sAddr, 5);
    VirtualProtect(sAddr, 5, oldProt, &oldProt);
#endif

    sPatched = false;
}
