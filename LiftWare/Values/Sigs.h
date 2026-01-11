#pragma once
#include <string_view>
#include <string>

class Sigs {
public:
	inline static constexpr std::string_view ClientInstanceUpdate = "48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 44 0F B6 FA"; //1.21.94
    inline static constexpr std::string_view ClientInstanceScreenModelSendChatMessage = "E8 ? ? ? ? 3C ? 75 ? 48 8B 8F"; //1.21.94
	inline static constexpr std::string_view GameMode_BaseUseItem = "E8 ? ? ? ? 84 C0 74 ? 48 8B ? 48 8B ? 48 8B ? ? ? ? ? FF 15 ? ? ? ? 48 85"; //1.21.94
	inline static constexpr std::string_view GameMode_GetDestroyRate = "E8 ? ? ? ? 0F 28 F8 49 8B 4E"; //1.21.94
	inline static constexpr std::string_view PlayerVtable = "48 8D 05 ? ? ? ? ? ? ? 89 9E ? ? ? ? 48 8D 05"; //1.21.94
	inline static constexpr std::string_view KeyPressFunc = "48 ? ? 48 ? ? ? 4C 8D 05 ? ? ? ? 89"; //1.21.94
	inline static constexpr std::string_view ActorSetRot = "48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 41 8B 58"; //1.21.94
	inline static constexpr std::string_view KeyMouseFunc = "48 8B C4 48 89 58 ? 48 89 68 ? 48 89 70 ? 57 41 54 41 55 41 56 41 57 48 83 EC ? 44 0F B7 BC 24 ? ? ? ? 48 8B D9"; //1.21.94
	inline static constexpr std::string_view CauseHurtCamFunc = "40 53 56 57 48 83 EC ? 48 8B F9 80 FA"; //1.21.94
	inline static constexpr std::string_view NoEatBobSig = "F3 0F 10 14 88 F3 0F 59 15"; //1.21.94
	inline static constexpr std::string_view OptionsVTable = "48 8D 05 ? ? ? ? ? ? ? 48 8B 89 ? ? ? ? BF"; //1.21.94
	inline static constexpr std::string_view crosshair = "48 8B C4 48 89 58 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 0F 29 70 ? 0F 29 78 ? 44 0F 29 40 ? 44 0F 29 48 ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4C 89 8D"; //1.21.94
	inline static const std::string version = "1.21.94";
};