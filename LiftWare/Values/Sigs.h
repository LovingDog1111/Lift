#pragma once
#include <string_view>

class Sigs {
public:
    inline static constexpr std::string_view ClientInstanceUpdate = "48 89 5C 24 ? 48 89 74 24 ? 55 57 41 54 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 44 0F";
    inline static constexpr std::string_view ClientInstanceScreenModelSendChatMessage = "E8 ? ? ? ? 3C 01 75 44 48 8B 8B ? ? ? ? 48 8B";
};
