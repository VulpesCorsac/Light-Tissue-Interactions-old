#pragma once

#include <random>
#include <stdlib.h>
#include <time.h>
#include <type_traits>

namespace Math_NS {
    template < typename T >
    static T random(T min, T max);

    template < typename T >
    T fastRandom();
}

/******************
 * IMPLEMENTATION *
 ******************/

template < typename T >
T Math_NS::random(T min, T max) {
    if constexpr (std::is_integral<T>())
        return static_cast<T>(random<double>(min, max));
    else {
        static thread_local std::random_device rd;
        static thread_local std::default_random_engine generator(rd());
        static thread_local std::uniform_real_distribution<T> distribution(0, 1);
        return min + (max - min) * distribution(generator);
    }
}

template < typename T >
T Math_NS::fastRandom() {
    return static_cast<T>(rand()) / static_cast<T>(RAND_MAX);
}
