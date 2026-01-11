#include "../../Memory/Memory.h"
#include "../../Libs/libhat/libhat.hpp"

class LevelRendererPlayer { //ima kill myself!~
public: 
    BUILD_ACCESS(Vector3<float>, cameraPos, Offsets::cameraPos);
    BUILD_ACCESS(Vector3<float>, origin, Offsets::origin)

    float getFovX() {
        return hat::member_at<float>(this, Offsets::FovXMember);
    }

    float getFovY() {
        return hat::member_at<float>(this, Offsets::FovYMember);
    }
};