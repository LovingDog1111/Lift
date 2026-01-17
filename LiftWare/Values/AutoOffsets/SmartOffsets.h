#pragma once
#include <cstdint>
#include <functional>
#include <iostream>

class SmartOffsets {
public:
    template<typename T>
    static T* findOffset(void* base, uintptr_t start, uintptr_t end, std::function<bool(T*)> validator) {
        for (uintptr_t offset = start; offset <= end; ++offset) {
            T* candidate = reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(base) + offset);
            try {
                if (validator(candidate)) {
                    return candidate;
                }
            }
            catch (...) {
                continue;
            }
        }
        return nullptr; 
    }
};

#define SMART_BUILD_ACCESS(type, name, startOffset, endOffset, validator) \
type* name() { \
    static type* cached = nullptr; \
    if (!cached) { \
        cached = SmartOffsets::findOffset<type>(this, startOffset, endOffset, validator); \
        if (!cached) { \
            std::cout << "Warning: Could not find valid offset for " #name "\n"; \
        } \
    } \
    return cached; \
}
