#pragma once
#include "../../FeatureBase/FeatureBase.h"

class AntiHit : public Feature {
public:
    AntiHit();
    void onSendPacket(Packet* packet) override;

private:
    int offsetMode = 0;
    int cycleTimer = 0;
    float offsetAmount = 1.0f;
};