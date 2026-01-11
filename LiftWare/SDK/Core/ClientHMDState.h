#pragma once
#include "../../Memory/Memory.h"
#include"../../Math/IncludeAll.h"

class ClientHMDState {
public:
	BUILD_ACCESS(glm::mat4, mLastLevelViewMatrix, Offsets::LastLevelViewMatrix);
	BUILD_ACCESS(glm::mat4, mLastLevelProjMatrix, Offsets::LastLevelProjMatrix); //:D
};