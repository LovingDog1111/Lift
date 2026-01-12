#include "KillAura.h"

KillAura::KillAura() : Feature("KillAura", "Automatically attacks nearby players.", Category::COMBAT) {
    std::vector<std::string> rots = { "Fast", "Slow" };
    registerSetting(new EnumSetting("Rotations", "Select your rotation mode", rots, &rotations, 0));
    registerSetting(new SliderSetting<float>("Reach", "How far to reach.", &reach, reach, 0.f, 20.f));
    std::vector<std::string> speeds = { "Slow", "Medium", "Fast" };
    registerSetting(new EnumSetting("Attack Speed", "Select your attack speed", speeds, &attackSpeed, 1));
    registerSetting(new BoolSetting("Strict", "Enable strict attack timing", &strict, false));
}

Actor* KillAura::getClosestTarget(Actor* localPlayer, float range) {
    Actor* closest = nullptr;
    float minDist = FLT_MAX;
    if (!Game::getClientInstance() || !Game::getLocalPlayer()) return nullptr;

    for (Actor* actor : localPlayer->level->getRuntimeActorList()) {
        if (!Targets::FilterInvalid(actor, false)) continue;

        Vector3<float> diff = actor->getPosition() - localPlayer->getPosition();
        float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
        if (dist > range) continue;

        if (dist < minDist) {
            minDist = dist;
            closest = actor;
        }
    }
    return closest;
}

void KillAura::onUpdateRotation(LocalPlayer* localPlayer) {
    if (!localPlayer) return;

    Actor* closest = getClosestTarget(localPlayer, reach + 0.5f);
    if (!closest) return;

    if (rotations == 0) {
        angles = localPlayer->getPosition().CalcAngle(closest->getPosition());
    }

    auto* rot = localPlayer->getActorRotationComponent();
    if (rot) {
        rot->presentRot.y = angles.y;
        rot->mYaw = angles.y;
        rot->mPitch = angles.x;
        rot->mOldYaw = angles.y;
        rot->mOldPitch = angles.x;
        rot->presentRot.x = angles.x;
    }
    auto* headrot = localPlayer->getActorHeadRotationComponent();
    if (headrot) {
        headrot->headYaw = angles.y;
    }
}

void KillAura::onNormalTick(LocalPlayer* localPlayer) {
    if (!localPlayer) return;

    Actor* target = getClosestTarget(localPlayer, reach);
    if (!target) return;

    static int delayTick = 0;
    int delay = 20; 

    if (attackSpeed == 0) delay = 15;
    else if (attackSpeed == 1) delay = 5;
    else if (attackSpeed == 2) delay = 0;

    if (delayTick <= 0) {
        localPlayer->swing();
        localPlayer->gameMode->attack(target);
        delayTick = delay;
    }
    else {
        delayTick--;
    }

    if (rotations == 1) {
        angles = localPlayer->getPosition().CalcAngle(target->getPosition());
    }
}
