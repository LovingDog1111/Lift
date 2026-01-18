#include "KillAura.h"

KillAura::KillAura() : Feature("KillAura", "Automatically attacks nearby players.", Category::COMBAT) {
    std::vector<std::string> rots = { "Fast", "Slow" };
    registerSetting(new EnumSetting("Rotations", "Select your rotation mode", rots, &rotations, 0));
    registerSetting(new SliderSetting<float>("Reach", "How far to reach.", &reach, reach, 0.f, 20.f));
    registerSetting(new BoolSetting("Exploit", "Exploit attack speed", &exploit, true));
}

std::vector<Actor*> KillAura::getTargets(Actor* localPlayer, float range) {
    std::vector<Actor*> targets;
    if (!Game::getClientInstance() || !Game::getLocalPlayer()) return targets;
    for (Actor* actor : localPlayer->level->getRuntimeActorList()) {
        if (!Targets::FilterInvalid(actor, false)) continue;

        Vector3<float> diff = actor->getPosition() - localPlayer->getPosition();
        float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
        if (dist > range) continue;
        targets.push_back(actor);
    }
    return targets;
}

void KillAura::onUpdateRotation(LocalPlayer* localPlayer) {
    if (!localPlayer) return;

    auto targets = getTargets(localPlayer, reach + 0.5f);
    if (targets.empty()) return;
    Actor* closest = *std::min_element(targets.begin(), targets.end(), [&](Actor* a, Actor* b) {
        Vector3<float> da = a->getPosition() - localPlayer->getPosition();
        Vector3<float> db = b->getPosition() - localPlayer->getPosition();
        float distA = std::sqrt(da.x * da.x + da.y * da.y + da.z * da.z);
        float distB = std::sqrt(db.x * db.x + db.y * db.y + db.z * db.z);
        return distA < distB;
        });

    if (rotations == 0) {
        angles = localPlayer->getPosition().CalcAngle(closest->getPosition());
        if (exploit) {
            for (int i = 0; i < 19; i++) {
                localPlayer->gameMode->attack(closest);
                localPlayer->swing();
            }
        }
    }

    auto* rot = localPlayer->getActorRotationComponent();
    if (rot) {
        rot->mYaw = angles.y;
        rot->mPitch = angles.x;
    }
    auto* headrot = localPlayer->getActorHeadRotationComponent();
    if (headrot) {
        headrot->headYaw = angles.y;
    }
}

void KillAura::onNormalTick(LocalPlayer* localPlayer) {
    if (!localPlayer) return;

    auto targets = getTargets(localPlayer, reach);
    if (targets.empty()) return;
    for (Actor* target : targets) {
        if (target) {
            localPlayer->swing();
            localPlayer->gameMode->attack(target);
            if (rotations == 1) {
                angles = localPlayer->getPosition().CalcAngle(target->getPosition());
            }
        }
    }
}
