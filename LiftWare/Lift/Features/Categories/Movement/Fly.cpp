#include "Fly.h"

Fly::Fly() : Feature("Fly", "Allows the player to fly freely.", Category::MOVEMENT) {
    registerSetting(new EnumSetting("Mode", "NULL", { "Motion", "Packet" }, &mode, 0));
    registerSetting(new SliderSetting<float>("Speed", "Speed of the flight.", &flySpeed, flySpeed, 0.f, 4.f));
    registerSetting(new SliderSetting<float>("Glide", "Glide.", &glide, glide, 0.f, 0.5f));
}

void Fly::onNormalTick(LocalPlayer* localPlayer) {
    Vector3<float>& velocity = localPlayer->getStateVectorComponent()->velocity;
    velocity = Vector3<float>(0.f, 0.f, 0.f);

    velocity.y += 0.0f;

    if (mode == 0) {
        if (Game::canUseMoveKeys()) {
            float yaw = localPlayer->getActorRotationComponent()->mYaw;

            bool isForward = Game::isKeyDown('W');
            bool isLeft = Game::isKeyDown('A');
            bool isBackward = Game::isKeyDown('S');
            bool isRight = Game::isKeyDown('D');
            bool isUp = Game::isKeyDown(VK_SPACE);
            bool isDown = Game::isKeyDown(VK_SHIFT);

            int moveX = 0;
            int moveY = 0;

            if (isRight)    moveX += 1;
            if (isLeft)     moveX -= 1;
            if (isForward)  moveY += 1;
            if (isBackward) moveY -= 1;

            if (isUp)   velocity.y += flySpeed;
            if (isDown) velocity.y -= flySpeed;

            if (moveX != 0 || moveY != 0) {
                float angleRad = std::atan2((float)moveX, (float)moveY);
                float angleDeg = angleRad * (180.f / PI);
                float finalYaw = yaw + angleDeg;
                float calcYaw = (finalYaw + 90.f) * (PI / 180.f);

                velocity.x = std::cos(calcYaw) * flySpeed;
                velocity.z = std::sin(calcYaw) * flySpeed;
            }
        }
    }
    else if (mode == 1) {
        if (Game::canUseMoveKeys()) {
            float yaw = localPlayer->getActorRotationComponent()->mYaw;

            bool isForward = Game::isKeyDown('W');
            bool isLeft = Game::isKeyDown('A');
            bool isBackward = Game::isKeyDown('S');
            bool isRight = Game::isKeyDown('D');
            bool isUp = Game::isKeyDown(VK_SPACE);
            bool isDown = Game::isKeyDown(VK_SHIFT);

            int moveX = 0;
            int moveY = 0;

            if (isRight)    moveX += 1;
            if (isLeft)     moveX -= 1;
            if (isForward)  moveY += 1;
            if (isBackward) moveY -= 1;

            if (isUp)   velocity.y += flySpeed;
            if (isDown) velocity.y -= flySpeed;

            if (moveX != 0 || moveY != 0) {
                float angleRad = std::atan2((float)moveX, (float)moveY);
                float angleDeg = angleRad * (180.f / PI);
                float finalYaw = yaw + angleDeg;
                float calcYaw = (finalYaw + 90.f) * (PI / 180.f);

                velocity.x = std::cos(calcYaw) * flySpeed;
                velocity.z = std::sin(calcYaw) * flySpeed;

                static bool sendNext = true;
                if (sendNext) {
                    auto genericAuthPacket = MinecraftPacket::createPacket(PacketID::PlayerAuthInput);
                    auto authPacket = std::reinterpret_pointer_cast<PlayerAuthInputPacket>(genericAuthPacket);
                    authPacket->mPosDelta = { 0.f, 0.f, 0.f };
                    authPacket->mPos = Game::getLocalPlayer()->getPosition();
                    authPacket->mClientTick = tick; 
                    authPacket->mRot.y = Game::getLocalPlayer()->getActorRotationComponent()->mYaw;
                    authPacket->mRot.x = Game::getLocalPlayer()->getActorRotationComponent()->mPitch;
                    authPacket->mYHeadRot = Game::getLocalPlayer()->getActorHeadRotationComponent()->headYaw;
                    authPacket->TicksAlive = tick2;

                    Game::getClientInstance()->getpacketSender()->sendToServer(authPacket.get());
                }
                sendNext = !sendNext;
            }
        }
    }
}

void Fly::onSendPacket(Packet* packet) {
    if (!packet || !Game::getLocalPlayer()) return;
    if (packet->getId() == PacketID::PlayerAuthInput) {
        auto paip = static_cast<PlayerAuthInputPacket*>(packet);
        if (paip) {
            tick = paip->mClientTick;
            tick2 = paip->TicksAlive;
        }
    }
}