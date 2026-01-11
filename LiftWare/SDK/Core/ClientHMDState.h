#pragma once
#include "../../Memory/Memory.h"
#include"../../Math/IncludeAll.h"

class ClientHMDState {
public:
	BUILD_ACCESS(glm::mat4, mLastLevelViewMatrix, 0xC8);
	BUILD_ACCESS(glm::mat4, mLastLevelProjMatrix, 0x148); //:D
};