#pragma once

#include <optional>
#include <string>

#define ALL_CONTAINER(c) begin(c), end(c)

namespace Utils_NS {
    /// returns c.size() casted to int to avoid warnings
    /// \param[in] c container to get size
    /// \return int size of the container
    template < typename Container >
    int isize(const Container& c) noexcept;

    /// returns string with replaced characters
    /// \param[in] from character to replace from
    /// \param[in] to replace found "from" to "to" if needed
    /// \return std::string result after replacing
    inline std::string replace(const std::string& s, char from, std::optional<char> to) noexcept;
}

/******************
 * IMPLEMENTATION *
 ******************/

template < typename Container >
int Utils_NS::isize(const Container& c) noexcept {
    return static_cast<int>(c.size());
}


std::string Utils_NS::replace(const std::string& s, char from, std::optional<char> to) noexcept {
    std::string result;

    for (const auto& c: s) {
        if (c == from) {
            if (to.has_value())
                result += to.value();
        } else
            result += c;
    }

    return result;
}
