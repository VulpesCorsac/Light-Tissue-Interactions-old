#pragma once

#define ALL_CONTAINER(c) begin(c), end(c)

namespace Utils_NS {
    /// returns c.size() casted to int to avoid warnings
    /// \param[in] c container to get size
    /// \return int size of the container
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
