#pragma once
#include "../../Math/IncludeAll.h"
#include "../../Memory/Memory.h"
#include "Packet.h"

class SetActorMotionPacket : public Packet {
   public:
    static const PacketID ID = PacketID::SetActorMotion;
    int64_t mRuntimeID;
   Vector3<float> mMotion;

    SetActorMotionPacket() = default;
};