#pragma once
#include <cstdint>
#include "AutoVTable/AutoVTableUpdater.h"

class VTables {
public:
    /* CLIENT INSTANCE BEGIN */
    inline static constexpr int ClientInstanceLocalPlayerOffset = 31; //1.21.94
    inline static constexpr int ClientInstanceGrabMouse = 347; //1.21.94
    inline static constexpr int ClientInstanceReleaseMouse = 348; //1.21.94
    inline static constexpr int ClientInstanceFocusMouse = 349; //1.21.94

    /* ACTOR BEGIN */
        inline static constexpr int ActorSwing = 111; //1.21.94

    /* LOCALPLAYER BEGIN */
    inline static constexpr int LocalPlayerDisplayClientMessage = 200; //1.21.94
    inline static constexpr int resetRot = AutoVTableUpdater::Update(389/*1.18.12 LocalPlayerDisplayClientMessage*/,/*1.21.94 LocalPlayerDisplayClientMessage*/200, /*1.18.12 resetrot*/373); //pro //1.21.94

    /* MOB BEGIN */
    inline static constexpr int MobSetSprinting = 140; //1.21.94

    /* LEVEL BEGIN */
    inline static constexpr int GetRuntimeActorList = 309; //1.21.94

    /* PLAYER BEGIN */
    inline static constexpr int PlayerPlayEmote = AutoVTableUpdater::Update(162, 149, 70); //1.21.94
    inline static constexpr int PlayerGetItemUseDuration = 149; //1.21.94
    
    /* MINECRAFT */
    inline static constexpr int PlayUI = 200; //1.21.94

    /* HOOKMANAGER (i dont know where to put this) */
    inline static constexpr int OnNormalTickIndex = 24; //1.21.94 localplayer
    inline static constexpr int GammaHook = 146; //1.21.94 options

    inline static const std::string version = "1.21.94";
};
