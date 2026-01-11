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
#include "Hooks/BrightnessFogColorHook.h"
#include "../Values/Sigs.h"

void HooksFactory::init() {
	MH_Initialize();

	AddHook<ClientInstanceUpdateHook>(Memory::FindSignature(Sigs::ClientInstanceUpdate, "ClientInstanceUpdate"));
	AddHook<BrightnessFogColorHook>(Memory::FindSignature("41 0f 10 08 48 8b c2 0f 28 d3"));
	AddHook<SendChatMessageHook>(Memory::getFuncFromCall(Memory::FindSignature(Sigs::ClientInstanceScreenModelSendChatMessage, "ClientInstanceScreenModelSendChatMessage")));
	AddHook<KeyMapHook>(Sigs::KeyPressFunc, "KeyPressFunc");
	AddHook<KeyMouseHook>(Sigs::KeyMouseFunc, "KeyMouseFunc");
	AddHook<ActorSetRotHook>(Sigs::ActorSetRot, "ActorSetRot");
	AddHook<HudCursorRendererHook>(Sigs::crosshair, "HudCursorRenderer_render");
	AddHook<HurtCamHook>(Sigs::CauseHurtCamFunc, "CauseHurtCamFunc");

	{
		uintptr_t** PlayerVTable = (uintptr_t**)(uintptr_t)Memory::getVtableFromSigSolstice(Sigs::PlayerVtable);
		AddHook<ActorNormalTickHook>(PlayerVTable, VTables::OnNormalTickIndex);
	}

	{
		uintptr_t** OptionsVtable = (uintptr_t**)(uintptr_t)Memory::GetVTableFromSignature(
			Sigs::OptionsVTable);
		AddHook<GammaHook>(OptionsVtable, VTables::GammaHook);
	}

	if (kiero::init(kiero::RenderType::D3D12) == kiero::Status::Success) { //Im pretty sure this never changes, so im not going to add it to the vtable class.
		uintptr_t** methodsTable = (uintptr_t**)kiero::getMethodsTable();
		AddHook<PresentHook>(methodsTable, 140);
		AddHook<ResizeBuffersHook>(methodsTable, 145);
	}
	else if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success) {
		uintptr_t** methodsTable = (uintptr_t**)kiero::getMethodsTable();
		AddHook<PresentHook>(methodsTable, 8);
		AddHook<ResizeBuffersHook>(methodsTable, 13);
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