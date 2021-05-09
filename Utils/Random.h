#pragma once

#include <random>
#include <cstdlib>
#include <stdlib.h>
#include <time.h>

template < typename T >
T random(T min, T max) {
    static std::random_device rd;
    static thread_local std::default_random_engine generator(rd());
    static std::uniform_real_distribution<T> distribution(min, max);
    return distribution(generator);
}

template < typename T >
T fast_random() {
    return static_cast<T>(rand()) / static_cast<T>(RAND_MAX);
}

int random_int(int min, int max) {
    return static_cast<int>(random<double>(min, max + 1));
}
