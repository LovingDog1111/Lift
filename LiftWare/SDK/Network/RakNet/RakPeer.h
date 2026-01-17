#pragma once
#include "../../Memory/Memory.h"
#include <stdint.h>

class RakPeer {
public:
    bool Ping(const char* addy, unsigned short port, bool b1, unsigned int u1) {
        return Memory::callVirtualFuncSolstice<bool>(38, this, "Ping", addy, port, b1, u1);
    }

    int GetAveragePing(std::string const& addressOrGuid) {
        return Memory::callVirtualFuncSolstice<int>(41,this, "GetAveragePing", addressOrGuid);
    }
};
