#pragma once

#include <algorithm>
#include <string>

#define ALL_CONTAINER(c) begin(c), end(c)

namespace Utils_NS {
    /// returns c.size() casted to int to avoid warnings
    /// \param[in] c container to get size
    /// \return int size of the container
    template < typename Container >
    int isize(const Container& c) noexcept;

    /// returns if value val is present in the container c
    /// \param[in] c container to search value
    /// \param[in] val value to search in container
    /// \return if value is present in the container
    template < typename Container, typename T >
    bool contains(const Container& c, const T& val) noexcept;
}

/******************
 * IMPLEMENTATION *
 ******************/

template < typename Container >
int Utils_NS::isize(const Container& c) noexcept {
    return static_cast<int>(c.size());
}

template < typename Container, typename T >
bool Utils_NS::contains(const Container& c, const T& val) noexcept {
    return c.find(val) != c.end();
}
