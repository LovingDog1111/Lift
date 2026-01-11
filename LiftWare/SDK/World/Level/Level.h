#pragma once
#include <string>
#include <cstdint>
#include <unordered_map>
#include <vector>
#include "../../Core/mce.h"
#include "../Actors/Actor.h"
#include "../../Memory/Memory.h"

class Actor;
struct Wrapper {
    uint8_t pad[0x10];
    void* ptrToT;

    template <typename T>
    T* get() const {
        return reinterpret_cast<T*>(ptrToT);
    }
};

class LevelData {
    BUILD_ACCESS(std::string, mName, 0x298);
};

class Level {
public:
    BUILD_ACCESS(std::shared_ptr<LevelData>, mLevelData, 0x90);
public:
    std::vector<Actor*> getRuntimeActorList() {
        std::vector<Actor*> listOut;
        Memory::callVirtualFuncSolstice<void>(309, this, "Level", &listOut);
        return listOut;
    }
};