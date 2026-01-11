#pragma once
#include "../../../Math/Vector3.h"

struct StateVectorComponent {
    Vector3<float> pos;
    Vector3<float> oldPos;
    Vector3<float> velocity;
};
