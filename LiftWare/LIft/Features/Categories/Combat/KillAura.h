#pragma once
#include "../../FeatureBase/FeatureBase.h"

class KillAura : public Feature {
public:
    KillAura();
    void onUpdateRotation(LocalPlayer* localPlayer) override;
    void onNormalTick(LocalPlayer* localPlayer) override;

private:
    Actor* getClosestTarget(Actor* localPlayer, float range);

    int rotations = 0;
    float reach = 6.f;
    Vector2<float> angles = { 0,0 };
    int attackSpeed = 1; 
    bool strict = false;
};
