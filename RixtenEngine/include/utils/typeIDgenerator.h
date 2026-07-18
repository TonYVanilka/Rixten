#pragma once
#include <cstddef>

// !can allow collisions in the same arrays
namespace typeIDgenerator {
    
    static size_t counter = 0;

    template<typename T>
    size_t id() {
        size_t value = counter++;
        return value;
    }
}

