#pragma once
#include <string>
#include <filesystem>
#include "../../Logs/Logger.h"

namespace FileFactory {

    std::string getRoamingStatePath();
    std::string getClientPath();

    inline bool doesFilePathExist(const std::string& path) {
        return std::filesystem::exists(path);
    }

    inline bool createPath(const std::string& path) {
        bool exists = doesFilePathExist(path);
        if (!exists) {
            std::filesystem::create_directories(path);
            Logger::Log("File path created: " + path);
            return true;
        }
        Logger::Log("File path already exists: " + path);
        return false;
    }

    inline bool deletePath(const std::string& path) {
        bool exists = doesFilePathExist(path);
        if (exists && std::filesystem::remove(path)) {
            Logger::Log("File path deleted: " + path);
            return true;
        }
        Logger::Log("File path did not exist: " + path);
        return false;
    }
}
