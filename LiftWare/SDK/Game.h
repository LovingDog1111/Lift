#pragma once
#include "../Memory/Memory.h"
#include "Core/ClientInstance.h"
#include "MCTextFormat.h"
#include "World/Actors/LocalPlayer.h"
#include "Network/MinecraftPacket.h"
#include "Network/PacketSender.h"
#include "Network/Packets/PlayerAuthInputPacket.h"
#include "Network/Packets/MovePlayerPacket.h"

namespace Game {
	extern ClientInstance* clientInstance;
	inline ClientInstance* getClientInstance() { return clientInstance; }
	extern void* keyMapPtr;
	extern void* keyMousePtr;
	extern int viewPerspectiveMode;

	bool isKeyDown(uint32_t key);
	bool isLeftClickDown();
	bool isRightClickDown();
	void DisplayClientMessage(const char* fmt, ...);

	inline bool canUseMoveKeys() {
		return clientInstance->mcGame->mouseGrabbed;
	}

	inline LocalPlayer* getLocalPlayer() {
		return clientInstance->getLocalPlayer();
	}
}
