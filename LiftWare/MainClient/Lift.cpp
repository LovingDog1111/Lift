#include "Lift.h"

void Lift::checkSigs() { //make sure everything is working and up to date!
    { 
        Memory::FindSignature(Sigs::ClientInstanceUpdate, "ClientInstance_Update");
        Memory::getFuncFromCall(Memory::FindSignature(Sigs::ClientInstanceScreenModelSendChatMessage, "ClientInstanceScreenModelSendChatMessage")); //this should work, right?
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
