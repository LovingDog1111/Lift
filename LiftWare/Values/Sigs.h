#pragma once
#include <string_view>
#include <string>

class Sigs {
public:
	inline static constexpr std::string_view ClientInstanceUpdate = "48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 55 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 44 0F B6 FA 48 8B F1 33 DB"; //updated
    inline static constexpr std::string_view ClientInstanceScreenModelSendChatMessage = "E8 ? ? ? ? 3C ? 0F 85 ? ? ? ? 48 8B 8F ? ? ? ? 48 8B 01 4C 89 65 ? 48 8D 55 ? 48 8B 40 ? FF 15 ? ? ? ? 90 48 8B 4D ? 48 85 C9 0F 84 ? ? ? ? 48 8D 45 ? 48 3B C8"; //updated what the fuck
	inline static constexpr std::string_view GameMode_BaseUseItem = "E8 ? ? ? ? 84 C0 74 ? 48 8B 07 48 8B CF 48 8B 80 ? ? ? ? FF 15 ? ? ? ? 48"; //updated ok
	inline static constexpr std::string_view GameMode_GetDestroyRate = "E8 ? ? ? ? 0F 28 F8 49 8B 4E"; //updated
	inline static constexpr std::string_view PlayerVtable = "48 8D 05 ? ? ? ? 48 89 06 89 9E ? ? ? ? 48 8D 05"; //updated..
	inline static constexpr std::string_view KeyPressFunc = "48 83 EC ? 48 83 02"; //idfk if this is right, check later.
	inline static constexpr std::string_view ActorSetRot = "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC ? 49 8B 00 48 8B E9"; //updated, not 100% sure.
	inline static constexpr std::string_view KeyMouseFunc = "48 8B C4 48 89 58 ? 48 89 68 ? 48 89 70 ? 57 41 54 41 55 41 56 41 57 48 83 EC ? 44 0F B7 BC 24"; //updated
	inline static constexpr std::string_view CauseHurtCamFunc = "40 53 56 57 48 83 EC ? 0F B6 C2"; //updated
	inline static constexpr std::string_view NoEatBobSig = "F3 0F 10 14 88 F3 0F 59 15"; //1.21.94
	inline static constexpr std::string_view OptionsVTable = "48 8D 05 ? ? ? ? 48 89 01 48 8B 89 ? ? ? ? BF ? ? ? ? 48 85 C9 74 ? 8B C7"; //updated
	inline static constexpr std::string_view crosshair = "48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 0F 29 B4 24 ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4C 89 8D ? ? ? ? 49 8B F8"; //updated lel
	inline static const std::string version = "1.21.132";
};