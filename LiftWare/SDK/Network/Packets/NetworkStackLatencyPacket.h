#pragma once
#include "../../Memory/Memory.h"
#include "Packet.h"
#include "../PacketHandlerDispatcherInstance.h"

class NetworkStackLatencyPacket : public ::Packet {
   public:
    static inline PacketID ID = PacketID::NetworkStackLatency;
    uint64_t mCreateTime;  // this+0x30
    bool mFromServer;      // this+0x38
};