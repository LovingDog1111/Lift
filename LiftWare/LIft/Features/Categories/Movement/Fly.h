#pragma once
#include "../../FeatureBase/FeatureBase.h"

class Fly : public Feature {
public:
    Fly();
    void onNormalTick(LocalPlayer* localPlayer) override;
private:
    float flySpeed;
};
