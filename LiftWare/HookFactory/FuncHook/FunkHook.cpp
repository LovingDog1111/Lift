#include "FuncHook.h"
#include <MinHook.h>

bool FuncHook::enableHook() {
	if (!enable) {
		if (!address) {
			Logger::ErrorLog("[%s] Invalid address!");
			return false;
		}
		if (MH_CreateHook((void*)address, func, reinterpret_cast<LPVOID*>(OriginFunc)) != MH_OK) {
			Logger::ErrorLog("[%s] Failed to create hook!");
			return false;
		}
		if (MH_EnableHook((void*)address) != MH_OK) {
			Logger::ErrorLog("[%s] Failed to enable hook!");
			return false;
		}

		enable = true;
		return true;
	}
	return true;
}

void FuncHook::onHookRequest() {
}