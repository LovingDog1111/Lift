#pragma once
#include <cstdint>
#include <string>

class Offsets {
public:
    inline static constexpr uintptr_t LastLevelViewMatrix = 0xC8;
    inline static constexpr uintptr_t LastLevelProjMatrix = 0x148;
    inline static constexpr uintptr_t mcGame = 0xC8;
    inline static constexpr uintptr_t minecraft = 0xD0;
    inline static constexpr uintptr_t guiData = 0x558;
    inline static constexpr uintptr_t HMDState = 0x5A8;
    inline static constexpr uintptr_t BadRefDef = 0x330;
    inline static constexpr uintptr_t FovX = 0x6F0;
    inline static constexpr uintptr_t FovY = 0x704;
    inline static constexpr uintptr_t MouseGrabbed = 0x1A8;
    inline static constexpr uintptr_t GameMode = 0xEC8;
    inline static constexpr uintptr_t Player = 0x8;
    inline static constexpr uintptr_t LastBreakPos = 0x10;
    inline static constexpr uintptr_t LastBreakFace = 0x1C;
    inline static constexpr uintptr_t LastDestroyProgress = 0x20;
    inline static constexpr uintptr_t DestroyProgress = 0x24;
    inline static constexpr uintptr_t MCTFBaseChar0 = 0xC2;
    inline static constexpr uintptr_t MCTFBaseChar1 = 0xA7;
    inline static constexpr uintptr_t PlayerMap = 0x1BC8;
    inline static constexpr uintptr_t Level = 0x250;
    inline static constexpr uintptr_t stateVector = 0x290;
    inline static constexpr uintptr_t aabbShape = 0x298;
    inline static constexpr uintptr_t rotation = 0x2A0;
    inline static const std::string version = "1.21.2";
};
