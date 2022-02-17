#pragma once

#include <random>
#include <stdlib.h>
#include <time.h>
#include <type_traits>

namespace Math_NS {
    template < typename T >
    static T random(T min = static_cast<T>(0), T max = static_cast<T>(1));

    template < typename T >
    static T randomC(T min = static_cast<T>(0), T max = static_cast<T>(1));
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
T Math_NS::randomC(T min, T max) {
    auto rnd = rand();
    rnd -= rnd == RAND_MAX ? 1 : 0;
    return min + (max - min) * static_cast<T>(rnd) / static_cast<T>(RAND_MAX);
}
