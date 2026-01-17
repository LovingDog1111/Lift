#include "Speed.h"

Speed::Speed() : Feature("Speed", "Speeds up the player.", Category::MOVEMENT) {
    registerSetting(new SliderSetting<float>("Speed", "How fast you go forward.", &speed, speed, 0.f, 2.f));
    registerSetting(new SliderSetting<float>("Strafespeed", "How fast you go sideways.", &strafeSpeed, strafeSpeed, 0.f, 2.f));
    registerSetting(new SliderSetting<float>("Downpull", "Extra force applied after jumping.", &downpull, downpull, 0.f, 1.f));
}

void Speed::onNormalTick(LocalPlayer* localPlayer) {
    Vector3<float>& velocity = localPlayer->getStateVectorComponent()->velocity;
    velocity = Vector3<float>(0.f, localPlayer->getStateVectorComponent()->velocity.y, 0.f);

    if (Game::canUseMoveKeys()) {
        float yaw = localPlayer->getActorRotationComponent()->mYaw;

        bool isForward = Game::isKeyDown('W');
        bool isLeft = Game::isKeyDown('A');
        bool isBackward = Game::isKeyDown('S');
        bool isRight = Game::isKeyDown('D');
        bool isMoving = (isForward || isLeft || isBackward || isRight);

        if (localPlayer->isOnGround() && isMoving) {
            localPlayer->Jump();
            currentDownpull = 0.f;
            downpullInterpolation = 0.f;
        }
        else {
            downpullInterpolation += 0.1f;
            if (downpullInterpolation > 1.f) downpullInterpolation = 1.f;
            currentDownpull = downpull * downpullInterpolation;
            velocity.y -= currentDownpull;
        }

        int moveX = 0;
        int moveY = 0;

        if (isRight) moveX += 1;
        if (isLeft) moveX -= 1;
        if (isForward) moveY += 1;
        if (isBackward) moveY -= 1;

        if (moveX != 0 || moveY != 0) {
            float angleRad = std::atan2((float)moveX, (float)moveY);
            float angleDeg = angleRad * (180.f / PI);
            float finalYaw = yaw + angleDeg;
            float calcYaw = (finalYaw + 90.f) * (PI / 180.f);

            float usedSpeed = speed;
            if (moveX != 0 && moveY == 0) {
                usedSpeed = strafeSpeed;
            }
            else if (moveX != 0 && moveY != 0) {
                usedSpeed = (speed + strafeSpeed) * 0.5f;
            }

            velocity.x = std::cos(calcYaw) * usedSpeed;
            velocity.z = std::sin(calcYaw) * usedSpeed;
        }
    }
}