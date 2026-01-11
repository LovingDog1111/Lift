#include "HooksFactory.h"
#include <kiero.h>
#include "Hooks/GammaHook.h"
#include "Hooks/ClientInstanceUpdateHook.h"
#include "Hooks/ActorNormalTickHook.h"
#include "Hooks/SendChatMessageHook.h"
#include "Hooks/KeyMapHook.h"
#include "../Values/Sigs.h"

void HooksFactory::init() {
	MH_Initialize();

	RequestHook<ClientInstanceUpdateHook>(Sigs::ClientInstanceUpdate);
	RequestHook<GammaHook>(Sigs::OptionsGetGamma);
	RequestHook<SendChatMessageHook>(Memory::getFuncFromCall(Memory::FindSignature(Sigs::ClientInstanceScreenModelSendChatMessage, "ClientInstanceScreenModelSendChatMessage")));
	RequestHook<KeyMapHook>(Sigs::KeyPressFunc);

	{
		uintptr_t** PlayerVTable = (uintptr_t**)Memory::GetVTableFromSignature(Sigs::PlayerVtable);
		RequestHook<ActorNormalTickHook>(PlayerVTable, 26);
	}

	for (auto& hook : hooksCache)
		hook->enableHook();

	for (auto& hook : hooksCache)
		delete hook;

	Logger::Log("Hook Factory Initialized");
	hooksCache.clear();
}

void HooksFactory::shutdown() {
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);
	MH_Uninitialize();
	kiero::shutdown();

	for (auto& funcHook : hooksCache) {
		delete funcHook;
	}
	hooksCache.clear();
}