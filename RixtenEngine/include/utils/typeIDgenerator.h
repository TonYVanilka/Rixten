#pragma once
#include <cstddef>

// !can allow collisions in the same arrays
namespace typeIDgenerator {
    
    template<typename Domain>
    struct Counter {
        inline static size_t counter = 0;
    };

    template <typename Domain, typename T>
    size_t id() {
        static size_t value = Counter<Domain>::counter++;
        return value;
    }
}