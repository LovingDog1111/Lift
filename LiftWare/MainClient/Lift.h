#pragma once
#include <string>
#include "../Files/Creation/FileFactory.h"
#include "../Logs/Logger.h"
#include "../Memory/Memory.h"
#include "../Values/Sigs.h"

class Lift {
private:
    static inline bool initialized = false;
public:
    static inline bool isInitialized() { return initialized; }
    static void init();
    static void checkSigs();
    static void shutdown();
};
