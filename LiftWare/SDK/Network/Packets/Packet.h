#pragma once
#include "../../Memory/Memory.h"
#include "PacketID.h"
#include <string>
#include "../PacketHandlerDispatcherInstance.h"

class Packet {
public:
    BUILD_ACCESS(PacketHandlerDispatcherInstance*, packetHandler, 0x20);
private:
    char pad_0x0[0x30];
public:
    PacketID getId() {
        return Memory::callVirtualFuncSolstice<PacketID>(1, this, "Packet::getId");
    }

    std::string getName() {
        return *Memory::callVirtualFuncSolstice<std::string*>(2, this, "Packet::getName", std::string());
    }
};