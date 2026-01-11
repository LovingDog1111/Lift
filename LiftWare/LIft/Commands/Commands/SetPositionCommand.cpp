#include "SetPositionCommand.h"
#include "../../Lift/Lift.h"
#include "../../Lift/Features/FeatureFactory.h"
#include <sstream>
#include <algorithm>

SetPositionCommand::SetPositionCommand()
    : CommandBase("setpos", "Sets player position", "<x> <y> <z>", { "pos" }) {
}

bool SetPositionCommand::execute(const std::vector<std::string>& args) {
    if (args.size() < 4)
        return false;

    float x, y, z;

    try {
        x = std::stof(args[1]);
        y = std::stof(args[2]);
        z = std::stof(args[3]);
    }
    catch (...) {
        Lift::displayMessage("%sInvalid coordinates!", MCTF::RED);
        return true;
    }

    auto player = Game::getLocalPlayer();
    if (!player) {
        Lift::displayMessage("%sPlayer not found!", MCTF::RED);
        return true;
    }

    //player->setPosition(Vector3<float>(x, y, z));
    /*
    Lift::displayMessage(
        "Position set to %s%.2f%s, %s%.2f%s, %s%.2f%s",
        MCTF::GRAY, x, MCTF::RESET,
        MCTF::GRAY, y, MCTF::RESET,
        MCTF::GRAY, z, MCTF::RESET
    );
    */
    Lift::displayMessage("SetPOS is a deleted function, needs replacement!");

    return true;
}
