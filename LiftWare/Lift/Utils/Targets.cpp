#include "Targets.h"

bool Targets::FilterInvalid(Actor* actor, bool mobsAllowed) {
    if (!actor) return false;
    if (!actor->getAABBShapeComponent()) return false;
    if (!actor->getStateVectorComponent()) return false;
    ActorTypeComponent* actorTypeComponent = actor->getActorTypeComponent();
    if (actorTypeComponent == nullptr)
        return false;
    uint32_t entityId = static_cast<uint32_t>(actorTypeComponent->id);
    if (actor == Game::getLocalPlayer()) return false;
    if (!mobsAllowed && actor->getActorTypeComponent()->id != 319) return false;
    return true;
}

Actor* Targets::GetClosestActorToPlayer(LocalPlayer* localPlayer, bool mobsAllowed, float range) {
    if (!localPlayer || !Game::getLocalPlayer() || !Game::getClientInstance()) return nullptr;
    Actor* closest = nullptr;
    float minDist = FLT_MAX;
    for (Actor* actor : localPlayer->level->getRuntimeActorList()) {
        if (!FilterInvalid(actor, mobsAllowed)) continue;

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
