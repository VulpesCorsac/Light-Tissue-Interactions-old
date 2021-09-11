#pragma once

#define ALL_CONTAINER(c) begin(c), end(c)

namespace Utils_NS {
    template < typename Container >
    int isize(const Container& c) noexcept;
}

/******************
 * IMPLEMENTATION *
 ******************/

template < typename Container >
int Utils_NS::isize(const Container& c) noexcept {
    return static_cast<int>(c.size());
}
