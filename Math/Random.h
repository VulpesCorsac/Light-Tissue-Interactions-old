#pragma once

#include <random>
#include <stdlib.h>
#include <time.h>

namespace Math_NS {
    template < typename T >
    T random(T min = static_cast<T>(0), T max = static_cast<T>(1));

    template < typename T >
    T fast_random();

    int random_int(int min, int max);
}

/******************
 * IMPLEMENTATION *
 ******************/

template < typename T >
T Math_NS::random(T min, T max) {
    static thread_local std::random_device rd;
    static thread_local std::default_random_engine generator(rd());
    static thread_local std::uniform_real_distribution<T> distribution(0, 1);
    return min + distribution(generator) * max;
}

template < typename T >
T Math_NS::fast_random() {
    return static_cast<T>(rand()) / static_cast<T>(RAND_MAX);
}

int Math_NS::random_int(int min, int max) {
    return static_cast<int>(random<double>(min, max + 1));
}
