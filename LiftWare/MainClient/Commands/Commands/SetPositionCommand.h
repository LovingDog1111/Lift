#pragma once
#include "CommandBase.h"

class SetPositionCommand : public CommandBase {
public:
    SetPositionCommand();
    bool execute(const std::vector<std::string>& args) override;
};
