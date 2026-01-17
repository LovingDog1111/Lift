#pragma once
#include "../../Math/IncludeAll.h"
#include "../../Memory/Memory.h"
#include "Packet.h"

class Event {
   public:
    explicit Event() = default;
};

class CancelableEvent : public Event {
   public:
    bool mCancelled = false;

    explicit CancelableEvent(bool cancelled = false) : mCancelled(cancelled) {}

    [[nodiscard]] bool isCancelled() const {
        return mCancelled;
    }
    void setCancelled(bool cancelled) {
        mCancelled = cancelled;
    }
    void cancel() {
        setCancelled(true);
    }
};