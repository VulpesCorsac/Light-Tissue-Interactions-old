#pragma once

#include <algorithm>
#include <optional>
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

    /// returns string with replaced characters
    /// \param[in] from character to replace from
    /// \param[in] to replace found "from" to "to" if needed
    /// \return std::string result after replacing
    inline std::string replace(const std::string& s, char from, std::optional<char> to) noexcept;

    /// returns string with all characters changed to lower case
    /// \param[in] s original string
    /// \return std::string result with all characters changed to lower case
    inline std::string to_lower(const std::string& s) noexcept;
    /// returns string with all characters changed to upper case
    /// \param[in] s original string
    /// \return std::string result with all characters changed to upper case
    inline std::string to_upper(const std::string& s) noexcept;
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

std::string Utils_NS::to_lower(const std::string& s) noexcept {
    auto result = s;
    std::transform(ALL_CONTAINER(result), result.begin(), [](unsigned char c) { return std::tolower(c); } );
    return result;
}

std::string Utils_NS::to_upper(const std::string& s) noexcept {
    auto result = s;
    std::transform(ALL_CONTAINER(result), result.begin(), [](unsigned char c) { return std::toupper(c); } );
    return result;
}
