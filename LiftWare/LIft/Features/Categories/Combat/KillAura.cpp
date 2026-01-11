#include "KillAura.h"

KillAura::KillAura() : Feature("Killaura", "Automatically attacks nearby players.", Category::COMBAT) {
    std::vector<std::string> rots = { "Fast", "Slow" };
    registerSetting(new EnumSetting("Rotations", "Select your rotation mode", rots, &rotations, 0));
}

std::vector<Actor*> KillAura::getTargets(Actor* localPlayer, float range) {
    std::vector<Actor*> targets;
    for (Actor* actor : localPlayer->level->getRuntimeActorList()) {
        if (!actor) continue;
        if (!actor->getaabbShape()) continue;
        if (!actor->getstateVector()) continue;
        if (actor == localPlayer) continue;

        Vector3<float> diff = actor->getPosition() - localPlayer->getPosition();
        float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
        if (dist > range) continue;
        targets.push_back(actor);
    }
    return targets;
}

void KillAura::onUpdateRotation(LocalPlayer* localPlayer) {
    if (!localPlayer) return;

    auto targets = getTargets(localPlayer, 6.2f);
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
    }

    auto* rot = localPlayer->getComponent<ActorRotationComponent>();
    if (rot) {
        rot->presentRot.y = angles.y;
        rot->presentRot.x = angles.x;
    }
    auto* headrot = localPlayer->getComponent<ActorHeadRotationComponent>();
    if (headrot) {
        headrot->headYaw = angles.y;
    }
}

void KillAura::onNormalTick(LocalPlayer* localPlayer) {
    if (!localPlayer) return;

    auto targets = getTargets(localPlayer, 6.f);
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
