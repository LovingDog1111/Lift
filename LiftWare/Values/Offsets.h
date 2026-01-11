#pragma once
#include <cstdint>
#include <string>

class Offsets {
public:
    inline static constexpr uintptr_t LastLevelViewMatrix = 0xC8; //1.21.94
    inline static constexpr uintptr_t LastLevelProjMatrix = 0x148; //1.21.94
    inline static constexpr uintptr_t mcGame = 0xD0; //1.21.94
    inline static constexpr uintptr_t minecraft = 0xD0; //1.21.94
    inline static constexpr uintptr_t guiData = 0x5B8; //1.21.94
    //inline static constexpr uintptr_t HMDState = 0x5A8; moved to vtables
    inline static constexpr uintptr_t BadRefDef = 0x330;
    inline static constexpr uintptr_t cameraPos = 0x664; //1.21.94
    inline static constexpr uintptr_t origin = 0x670;   //1.21.94
    inline static constexpr uintptr_t FovXMember = 0x1020; //1.21.94
    inline static constexpr uintptr_t FovYMember = 0x1034; //1.21.94

    inline static constexpr uintptr_t MouseGrabbed = 0x1B8; //1.21.94
    inline static constexpr uintptr_t GameMode = 0xA88; //1.21.94
    inline static constexpr uintptr_t Player = 0x8; //1.21.94
    inline static constexpr uintptr_t LastBreakPos = 0x10; //1.21.94
    inline static constexpr uintptr_t LastBreakFace = 0x1C; //1.21.94
    inline static constexpr uintptr_t LastDestroyProgress = 0x20; //1.21.94
    inline static constexpr uintptr_t DestroyProgress = 0x24; //1.21.94
    inline static constexpr uintptr_t MCTFBaseChar0 = 0xC2;
    inline static constexpr uintptr_t MCTFBaseChar1 = 0xA7;
    inline static constexpr uintptr_t PlayerMap = 0x1BC8;
   // inline static constexpr uintptr_t Level = 0x1D8; //1.21.94
    inline static constexpr uintptr_t stateVector = 0x290; //1.21.94
    //inline static constexpr uintptr_t aabbShape = 0x298; moved to components
    //inline static constexpr uintptr_t rotation = 0x2A0; movded to components
    inline static const std::string version = "1.21.94";
};
