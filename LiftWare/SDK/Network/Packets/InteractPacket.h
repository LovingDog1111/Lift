#pragma once
#include "../../Math/IncludeAll.h"
#include "../../Memory/Memory.h"
#include "Packet.h"

enum class InteractAction { RIGHT_CLICK = 1, LEFT_CLICK = 2, LEAVE_VEHICLE = 3, MOUSEOVER = 4 };

class InteractPacket : public ::Packet {
   public:
    static const PacketID ID = PacketID::Interact;

    enum class Action : unsigned char {
        Invalid = 0x0,
        RightClick = 0x1,
        LeftClick = 0x2,
        StopRiding = 0x3,
        InteractUpdate = 0x4,
        NpcOpen = 0x5,
        OpenInventory = 0x6,
    };

    Action mAction;      // this+0x30
    int mTargetId;  // this+0x38
    Vector3<float> mPos;      // this+0x40

    InteractPacket& operator=(InteractPacket const&);
    InteractPacket(InteractPacket const&);
};
