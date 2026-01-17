#pragma once
#include "../Lift.h"
#include "../../SDK/Game.h"

class Targets {
public:
    static bool FilterInvalid(Actor* actor, bool mobsAllowed);
    static Actor* GetClosestActorToPlayer(LocalPlayer* localPlayer, bool mobsAllowed = false, float range = 20.f);
};