#pragma once
#include <cstdint>
#include "AutoVTable/AutoVTableUpdater.h"

class VTables {
public:
    /* CLIENT INSTANCE BEGIN */
    inline static constexpr int ClientInstanceLocalPlayerOffset = 29;
    inline static constexpr int ClientInstancePlayUI = 338;
    inline static constexpr int ClientInstanceGrabMouse = 332;
    inline static constexpr int ClientInstanceReleaseMouse = 333;
    inline static constexpr int ClientInstanceFocusMouse = 334;
    /* CLIENT INSTANCE END */

    /* ACTOR BEGIN */
        inline static constexpr int ActorSwing = 117;
    /* ACTOR END */

    /* LOCALPLAYER BEGIN */
    inline static constexpr int LocalPlayerDisplayClientMessage = 213;
    inline static constexpr int resetRot = AutoVTableUpdater::Update(389/*1.18.12 LocalPlayerDisplayClientMessage*/,/*1.21.2 LocalPlayerDisplayClientMessage*/213, /*1.18.12 resetrot*/373); //pro
    /* LOCALPLAYER END */

    /* MOB BEGIN */
    inline static constexpr int MobSetSprinting = 153;
    /* MOB END */

    /* PLAYER BEGIN */
    inline static constexpr int PlayerPlayEmote = 70;
    inline static constexpr int PlayerGetItemUseDuration = 162;
    /* PLAYER END */

    inline static const std::string version = "1.21.2";
};
