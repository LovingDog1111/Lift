#pragma once
#include <string>
#include "Offsets.h"
#include "Sigs.h"
#include "VTables.h"
#include <iostream>

class Client {
public:
    inline static const std::string TargetVersion = "1.21.94";

    bool isUpToDate() {
        if (TargetVersion == Offsets::version &&
            TargetVersion == Sigs::version &&
            TargetVersion == VTables::version) {
            return true;
        }
        else {
            std::cout << "[ClientVersion] Warning: Client is outdated! Target: "
                << TargetVersion << ", Actual: Offsets " << Offsets::version
                << ", Sigs " << Sigs::version
                << ", VTables " << VTables::version << std::endl;
            return false;
        }
    }
};
