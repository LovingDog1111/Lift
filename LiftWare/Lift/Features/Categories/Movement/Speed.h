#pragma once
#include "../../FeatureBase/FeatureBase.h"

class Speed : public Feature {
private:
    float speed = 0.5f;
    float downpull = 0.1f;
    float strafeSpeed = 0.5f;
    float currentDownpull = 0.f;
    float downpullInterpolation = 0.f;

public:
    Speed();
    void onNormalTick(LocalPlayer* localPlayer) override;
};