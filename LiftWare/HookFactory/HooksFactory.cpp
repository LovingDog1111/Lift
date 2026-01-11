#include "HooksFactory.h"
#include <kiero.h>
#include "Hooks/GammaHook.h"
#include "Hooks/ClientInstanceUpdateHook.h"
#include "Hooks/ActorNormalTickHook.h"
#include "Hooks/SendChatMessageHook.h"
#include "Hooks/KeyMapHook.h"
#include "Hooks/KeyMouseHook.h"
#include "Hooks/ActorSetRotHook.h"
#include "Hooks/ResizeBuffersHook.h"
#include "Hooks/PresentHook.h"
#include "Hooks/HurtCamHook.h"
#include "Hooks/HudCursorRendererHook.h"
#include "../Lift/Features/FeatureFactory.h"
#include "../Values/Sigs.h"

void HooksFactory::init() {
	MH_Initialize();

	RequestHook<ClientInstanceUpdateHook>(Sigs::ClientInstanceUpdate, "ClientInstanceUpdate");
	RequestHook<GammaHook>(Sigs::OptionsGetGamma, "Options_GetGamma");
	RequestHook<SendChatMessageHook>(Memory::getFuncFromCall(Memory::FindSignature(Sigs::ClientInstanceScreenModelSendChatMessage, "ClientInstanceScreenModelSendChatMessage")));
	RequestHook<KeyMapHook>(Sigs::KeyPressFunc, "KeyPressFunc");
	RequestHook<KeyMouseHook>(Sigs::KeyMouseFunc, "KeyMouseFunc");
	RequestHook<ActorSetRotHook>(Sigs::ActorSetRot, "ActorSetRot");
	RequestHook<HudCursorRendererHook>(Sigs::HudCursorRenderer_render, "HudCursorRenderer_render");
	RequestHook<HurtCamHook>(Sigs::CauseHurtCamFunc, "CauseHurtCamFunc");

	{
		uintptr_t** PlayerVTable = (uintptr_t**)Memory::GetVTableFromSignature(Sigs::PlayerVtable);
		RequestHook<ActorNormalTickHook>(PlayerVTable, 26);
	}


	if (kiero::init(kiero::RenderType::D3D12) == kiero::Status::Success) {
		uintptr_t** methodsTable = (uintptr_t**)kiero::getMethodsTable();
		RequestHook<PresentHook>(methodsTable, 140);
		RequestHook<ResizeBuffersHook>(methodsTable, 145);
	}
	else if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success) {
		uintptr_t** methodsTable = (uintptr_t**)kiero::getMethodsTable();
		RequestHook<PresentHook>(methodsTable, 8);
		RequestHook<ResizeBuffersHook>(methodsTable, 13);
	}
	else {
		return;
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