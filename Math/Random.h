#pragma once

#include <random>
#include <cstdlib>
#include <stdlib.h>
#include <time.h>

namespace Math_NS {
    template < typename T >
    T random(T min, T max);

    template < typename T >
    T fast_random();

    int random_int(int min, int max);
}

/******************
 * IMPLEMENTATION *
 ******************/

template < typename T >
T Math_NS::random(T min, T max) {
    using namespace std;

    static random_device rd;
    static thread_local default_random_engine generator(rd());
    static uniform_real_distribution<T> distribution(min, max);
    return distribution(generator);
}

template < typename T >
T Math_NS::fast_random() {
    return static_cast<T>(rand()) / static_cast<T>(RAND_MAX);
}

int Math_NS::random_int(int min, int max) {
    return static_cast<int>(random<double>(min, max + 1));
}
