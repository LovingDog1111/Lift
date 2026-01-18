#include "Fly.h"
#include <cmath>

Fly::Fly() : Feature("Fly", "Allows the player to fly freely.", Category::MOVEMENT) {
    registerSetting(new SliderSetting<float>("Speed", "Speed of the flight.", &flySpeed, flySpeed, 0.f, 2.f));
}

void Fly::onNormalTick(LocalPlayer* localPlayer) {
    Vector3<float>& velocity = localPlayer->getStateVectorComponent()->velocity;
    velocity = Vector3<float>(0.f, 0.f, 0.f);

    velocity.y += 0.0f;

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
