#pragma once
#include "../../FeatureBase/FeatureBase.h"

class Fly : public Feature {
public:
    Fly();
    void onNormalTick(LocalPlayer* localPlayer) override;
    void onSendPacket(Packet* packet) override;
private:
    float flySpeed = 1.0f;
    float glide = 0.0f;
    int mode = 0;
    int tick = 0;
    int tick2 = 0;
};
