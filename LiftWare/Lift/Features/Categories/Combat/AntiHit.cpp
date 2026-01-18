#include "AntiHit.h"

AntiHit::AntiHit() : Feature("AntiHit", "Moves position to avoid hits", Category::COMBAT) {
    registerSetting(new SliderSetting<float>("Offset", "Offset amount", &offsetAmount, offsetAmount, 0.1f, 5.0f));
}

void AntiHit::onSendPacket(Packet* packet) {
    if (!packet) return;

    cycleTimer++;
    if (cycleTimer >= 2) {
        offsetMode = (offsetMode + 1) % 4;
        cycleTimer = 0;
    }

    if (packet->getId() == PacketID::PlayerAuthInput) {
        auto paip = static_cast<PlayerAuthInputPacket*>(packet);
        if (paip) {
            if (offsetMode == 0) {
                paip->mPos.x += offsetAmount;
            }
            else if (offsetMode == 1) {
                paip->mPos.z += offsetAmount;
            }
            else if (offsetMode == 2) {
                paip->mPos.x -= offsetAmount;
            }
            else if (offsetMode == 3) {
                paip->mPos.z -= offsetAmount;
            }
            Actor* target = Targets::GetClosestActorToPlayer(Game::getLocalPlayer(), false, 5.0f);
            if (target) {
                paip->mPos.y -= 2.f;
            }
        }
    }

    if (packet->getId() == PacketID::MovePlayer) {
        auto mpp = static_cast<MovePlayerPacket*>(packet);
        if (mpp) {
            if (offsetMode == 0) {
                mpp->mPos.x += offsetAmount;
            }
            else if (offsetMode == 1) {
                mpp->mPos.z += offsetAmount;
            }
            else if (offsetMode == 2) {
                mpp->mPos.x -= offsetAmount;
            }
            else if (offsetMode == 3) {
                mpp->mPos.z -= offsetAmount;
            }
            Actor* target = Targets::GetClosestActorToPlayer(Game::getLocalPlayer(), false, 5.0f);
            if (target) {
                mpp->mPos.y -= 2.f;
            }
        }
    }
}