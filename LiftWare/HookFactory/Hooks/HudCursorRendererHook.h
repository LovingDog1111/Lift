#pragma once
#include "../FuncHook/FuncHook.h"
#include "../../Lift/Lift.h"
#include "../../Lift/Features/FeatureFactory.h"
#include "../../Lift/Commands/CommandFactory.h"

class HudCursorRenderer;
class UIControl;
class MinecraftUIRenderContext;
class RectangleArea;
class HudCursorRendererHook : public FuncHook {
private:
	using func_t = void* (__thiscall*)(HudCursorRenderer*, MinecraftUIRenderContext*, ClientInstance*, UIControl*, int, RectangleArea*);
	static inline func_t oFunc;

	static void* HudCursorRendererCallback(HudCursorRenderer* _this, MinecraftUIRenderContext* renderCtx, ClientInstance* client, UIControl* owner, int pass, RectangleArea* renderAABB) {

		static NoCrosshair* nocrosshair = FeatureFactory::getFeature<NoCrosshair>();
		if (nocrosshair && nocrosshair->isEnabled() && Game::canUseMoveKeys()) {
			return nullptr;
		}
		return oFunc(_this, renderCtx, client, owner, pass, renderAABB);
	}
public:
	HudCursorRendererHook() {
		OriginFunc = (void*)&oFunc;
		func = (void*)&HudCursorRendererCallback;
	}
};
