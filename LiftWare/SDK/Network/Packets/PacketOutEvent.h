#pragma once
#include "../../Math/IncludeAll.h"
#include "../../Memory/Memory.h"
#include "Packet.h"
#include <map>
#include <vector>
#include <memory>
#include "Event.h"

class PacketOutEvent : public CancelableEvent {
   public:
    Packet* mPacket;

    explicit PacketOutEvent(Packet* packet) : CancelableEvent() {
        this->mPacket = packet;
    }

    template <typename T>
    T* getPacket() const {
        return reinterpret_cast<T*>(mPacket);
    }
};