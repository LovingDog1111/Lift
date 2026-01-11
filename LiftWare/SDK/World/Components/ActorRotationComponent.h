#pragma once
#include "../../../Math/Vector2.h"

struct ActorRotationComponent {
public:
    float mPitch;
    float mYaw;
    float mOldPitch;
    float mOldYaw;
    Vector2<float> presentRot;
    Vector2<float> prevRot;
};
    
static_assert(sizeof(ActorRotationComponent) == 0x20);