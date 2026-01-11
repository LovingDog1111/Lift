#include "CommandFactory.h"
#include "../../MainClient/Lift.h"
#include "../../MainClient/Features/FeatureFactory.h"
#include "Commands/BindCommand.h"
#include "Commands/SetPositionCommand.h"
#include <sstream>

void CommandFactory::init() {
    commandList.push_back(new BindCommand());
    commandList.push_back(new SetPositionCommand());
}

void CommandFactory::shutdown() {
	for (auto& command : commandList) {
		delete command;
		command = nullptr;
	}
	commandList.clear();
}

void CommandFactory::execute(const std::string& message) {
    if (message.empty() || message[0] != prefix)
        return;

    std::string currentMessage = message.substr(1);

    std::vector<std::string> args;
    std::stringstream ss(currentMessage);
    std::string word;

    while (ss >> word) {
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
        args.push_back(word);
    }

    if (args.empty())
        return;

    std::string cmd = args[0];

    for (auto& command : commandList) {
        std::vector<std::string> checkList;
        checkList.push_back(command->name);

        for (const auto& alias : command->aliases)
            checkList.push_back(alias);

        for (auto check : checkList) {
            std::transform(check.begin(), check.end(), check.begin(), ::tolower);

            if (cmd == check) {
                if (!command->execute(args)) {
                    Lift::displayMessage(
                        "%sUsage: %s%c%s %s",
                        MCTF::BOLD, MCTF::RESET, prefix,
                        command->name.c_str(),
                        command->usage.c_str()
                    );
                }
                return;
            }
        }
    }

    Lift::displayMessage(
        "Couldn't find command %s%s%s!",
        MCTF::GRAY, cmd.c_str(), MCTF::WHITE
    );
}
