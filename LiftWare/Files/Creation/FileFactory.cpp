#include "FileFactory.h"

std::string FileFactory::getRoamingStatePath() {
	static std::string RoamingStatePath = (getenv("AppData") + std::string("\\..\\Local\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\RoamingState\\"));
	return RoamingStatePath;
}

std::string FileFactory::getClientPath() {
	static std::string ClientPath = getRoamingStatePath() + "Lift\\";
	return ClientPath;
}