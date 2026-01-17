#pragma once
#include "../../Math/IncludeAll.h"
#include "../../Memory/Memory.h"
#include "Packet.h"
#include <memory>

class LoginPacket : public ::Packet {
   public:
    int mClientNetworkVersion;                                    // this+0x30
    std::unique_ptr<class ConnectionRequest> mConnectionRequest;  // this+0x38
};