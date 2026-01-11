#include "Lift.h"
#include "../HookFactory/HooksFactory.h"
#include "../Values/Client.h"

void Lift::checkPads() { //we.. cant really check this, so this will have to do. its more so for looks.
    auto checkPad = [](uintptr_t padValue, const std::string& name) {
        if (padValue == 0) {
            Logger::ErrorLog("Pad not valid: " + name);
        }
        else {
            Logger::GoodLog("Pad valid: " + name + " -> 0x" + std::to_string(padValue));
        }
        };

    checkPad(Pads::Minecraft_pad_0x0, "Minecraft_pad_0x0");
    checkPad(Pads::GuiData_pad_0x0, "GuiData_pad_0x0");

    const std::string expectedVersion = Pads::version;
    const std::string clientVersion = Client::TargetVersion;
    if (clientVersion != expectedVersion) {
        Logger::ErrorLog("Client version mismatch! Expected: " + expectedVersion + ", Found: " + clientVersion);
    }
    else {
        Logger::GoodLog("Client version valid: " + clientVersion);
    }
}

void Lift::checkSigs() { //make sure everything is working and up to date!
    { 
       // Memory::FindSignature(Sigs::ClientInstanceUpdate, "ClientInstance_Update");
        Memory::getFuncFromCall(Memory::FindSignature(Sigs::ClientInstanceScreenModelSendChatMessage, "ClientInstanceScreenModelSendChatMessage")); //this should work, right?
        Memory::FindSignature(Sigs::GameMode_BaseUseItem, "GameMode_BaseUseItem");
        Memory::FindSignature(Sigs::GameMode_GetDestroyRate, "GameMode_GetDestroyRate");
		Memory::FindSignature(Sigs::PlayerVtable, "PlayerVTable");
		Memory::FindSignature(Sigs::KeyPressFunc, "KeyMap");
		Memory::FindSignature(Sigs::ActorSetRot, "ActorSetRot");
    }
}

void Lift::checkOffsets() {
    //unused function as of now, will figure this out later!
}

void Lift::init() {
    std::string path = FileFactory::getClientPath();
    if (!FileFactory::doesFilePathExist(path))
        FileFactory::createPath(path);

    std::filesystem::path logsFolder = std::filesystem::path(path) / "Logs";
    if (!std::filesystem::exists(logsFolder)) {
        std::filesystem::create_directory(logsFolder);
    }

    std::filesystem::path logFile = logsFolder / "Logs.txt";
    if (!std::filesystem::exists(logFile)) {
        std::ofstream(logFile.string());
    }

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
	Game::getClientInstance()->getguiData()->displayMessage(messageStr);
}