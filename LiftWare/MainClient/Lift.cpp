#include "Lift.h"
#include "../HookFactory/HooksFactory.h"

void Lift::checkSigs() { //make sure everything is working and up to date!
    { 
        Memory::FindSignature(Sigs::ClientInstanceUpdate, "ClientInstance_Update");
        Memory::getFuncFromCall(Memory::FindSignature(Sigs::ClientInstanceScreenModelSendChatMessage, "ClientInstanceScreenModelSendChatMessage")); //this should work, right?
        Memory::FindSignature(Sigs::OptionsGetGamma, "Options_GetGamma");
        Memory::FindSignature(Sigs::OptionsGetViewPerspective, "Options_GetViewPerspective");
        Memory::FindSignature(Sigs::Actor_SetPos, "Actor_SetPos");
        Memory::FindSignature(Sigs::GameMode_BaseUseItem, "GameMode_BaseUseItem");
        Memory::FindSignature(Sigs::GameMode_GetDestroyRate, "GameMode_GetDestroyRate");
		Memory::FindSignature(Sigs::PlayerVtable, "PlayerVTable");
		Memory::FindSignature(Sigs::KeyPressFunc, "KeyMap");
    }
}

void Lift::init() {
    std::string path = FileFactory::getClientPath();
    if (!FileFactory::doesFilePathExist(path))
        FileFactory::createPath(path);

    Logger::Log("Lift has been initialized!");
    initialized = true;
}

void Lift::shutdown() {
    initialized = false;
}

void Lift::displayMessage(const char* fmt, ...) {
	LocalPlayer* localPlayer = Game::getLocalPlayer();
	if (localPlayer == nullptr)
		return;

	va_list arg;
	va_start(arg, fmt);
	char message[300];
	vsprintf_s(message, 300, fmt, arg);
	va_end(arg);

	static std::string headerMessage;
	if (headerMessage.empty()) {
		char headerCStr[50];
		sprintf_s(headerCStr, 50, "%s[%sLift%s] %s", MCTF::GRAY, MCTF::GREEN, MCTF::GRAY, MCTF::WHITE);
		headerMessage = std::string(headerCStr);
	}

	std::string messageStr = headerMessage + std::string(message);
	localPlayer->displayClientMessage(messageStr);
}