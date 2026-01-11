#pragma once
#include <string_view>
#include <string>

class Sigs {
public:
    inline static constexpr std::string_view ClientInstanceUpdate = "48 89 5C 24 ? 48 89 74 24 ? 55 57 41 54 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 44 0F";
    inline static constexpr std::string_view ClientInstanceScreenModelSendChatMessage = "E8 ? ? ? ? 3C 01 75 44 48 8B 8B ? ? ? ? 48 8B";
	inline static constexpr std::string_view OptionsGetViewPerspective = "48 83 EC ? 48 8B 01 48 8D 54 ? ? 41 B8 03 00 00 00";
	inline static constexpr std::string_view OptionsGetGamma = "48 83 EC ? 80 B9 ? ? ? ? ? 48 8D 54 24 ? 48 8B 01 48 8B 40 ? 74 ? 41 B8 ? ? ? ? FF 15 ? ? ? ? 48 8B 10 48 85 D2 74 ? 48 8B 42 ? 48 8B 88 ? ? ? ? 48 85 C9 74 ? E8 ? ? ? ? 48 83 C4 ? C3 F3 0F 10 42 ? 48 83 C4 ? C3 41 B8 ? ? ? ? FF 15 ? ? ? ? 48 8B 10 48 85 D2 75 ? E8 ? ? ? ? CC E8 ? ? ? ? CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC F3 0F 11 4C 24";
	inline static constexpr std::string_view Actor_SetPos = "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 30 48 8B DA 48 8B F9 48 8B CB 48 8D";
	inline static constexpr std::string_view GameMode_BaseUseItem = "48 89 5C 24 ? 48 89 74 24 ? 55 57 41 56 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 60 48 8B F2 48 8B F9 45 33 F6 4C 89 75 08 48 8D 4D 08 E8 ? ? ? ? 90 48";
	inline static constexpr std::string_view GameMode_GetDestroyRate = "48 89 5C 24 ? 57 48 83 EC 30 48 8B F9 0F 29 74 24 ? 48 8B 49 08 E8";
	inline static constexpr std::string_view PlayerVtable = "48 8D 05 ? ? ? ? 48 89 01 B8 ? ? ? ? 8D 50 FA 44 8D 48 ? 44 8D 40 ? 66 89 44 ? ? E8 ? ? ? ? 48 8B 8B";
	inline static constexpr std::string_view KeyPressFunc = "48 ? ? 48 ? ? ? 4C 8D 05 ? ? ? ? 89";
	inline static constexpr std::string_view ActorSetRot = "4C 8B DC 49 89 73 10 49 89 7B 20 55 49 8D 6B A1 48 81 EC ? ? ? ? 48 8B";
	inline static constexpr std::string_view TryGetActorHeadRotationComponent = "E8 ? ? ? ? 48 85 C0 74 57 F3 0F";
	inline static constexpr std::string_view KeyMouseFunc = "48 8B C4 48 89 58 ? 48 89 68 ? 48 89 70 ? 57 41 54 41 55 41 56 41 57 48 83 EC ? 44 0F B7 BC 24 ? ? ? ? 48 8B D9";
	inline static constexpr std::string_view HudCursorRenderer_render = "48 8B C4 48 89 58 20 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 68 FA";
	inline static constexpr std::string_view CauseHurtCamFunc = "48 89 5C 24 ? 57 48 83 EC 30 48 8B F9 80 FA";
	inline static constexpr std::string_view NoEatBobSig = "F3 0F 10 0C 88 F3 0F 59 0D ? ? ? ? 8B 05";
	inline static const std::string version = "1.21.2";
};