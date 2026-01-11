#pragma once
#include <string>
#include "Offsets.h"
#include "Sigs.h"
#include "VTables.h"
#include <iostream>

class ClientVersion {
public:
    std::string TargetVersion = "1.21.2";

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
