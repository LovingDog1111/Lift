#pragma once
#include "../../Math/IncludeAll.h"
#include "../../Memory/Memory.h"
#include "Packet.h"

class PlaySoundPacket : public ::Packet {
   public:
    static inline PacketID ID = PacketID::PlaySound;

    std::string mName;  // this+0x30
    BlockPos mPos;      // this+0x50
    float mVolume;      // this+0x5C
    float mPitch;       // this+0x60
};