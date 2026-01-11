#pragma once
#include <cstdint>

class AutoVTableUpdater {
public:
    static inline constexpr int Update(int oldKnown, int newKnown, int oldTarget) { // guesses new vtable index using delta from known mapping
        return oldTarget + (newKnown - oldKnown) + 7;
    }
};
