#pragma once

#include "Utils.h"

#include <set>

namespace Utils_NS {
    /// change string with replaced characters
    /// \param[in,out] s string to replace characters in
    /// \param[in] from character to replace from
    /// \param[in] to replace found "from" to "to" if needed
    inline void replace_inplace(std::string& s, char from, std::optional<char> to) noexcept;
    /// returns string with replaced characters
    /// \param[in] s string to replace characters in
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

    inline void to_lower_inplace(std::string& s) noexcept;
    inline void to_upper_inplace(std::string& s) noexcept;

    inline std::set<std::string> getAllVariants(const std::string& data) noexcept;

    inline std::vector<std::string> split(const std::string& data, const char& c) noexcept;

    inline std::string join(const std::vector<std::string>& data, const char& c) noexcept;

    inline void prepend_inplace(std::vector<std::string>& data, const std::string& s) noexcept;

    inline std::vector<std::string> prepend(const std::vector<std::string>& data, const std::string& s) noexcept;
}

/******************
 * IMPLEMENTATION *
 ******************/

void Utils_NS::replace_inplace(std::string& s, char from, std::optional<char> to) noexcept {
    s = replace(s, from, to);
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

void Utils_NS::to_lower_inplace(std::string& s) noexcept {
    std::transform(ALL_CONTAINER(s), s.begin(), [](unsigned char c) { return std::tolower(c); } );
}

std::string Utils_NS::to_lower(const std::string& s) noexcept {
    auto result = s;
    std::transform(ALL_CONTAINER(result), result.begin(), [](unsigned char c) { return std::tolower(c); } );
    return result;
}

void Utils_NS::to_upper_inplace(std::string& s) noexcept {
    std::transform(ALL_CONTAINER(s), s.begin(), [](unsigned char c) { return std::toupper(c); } );
}

std::string Utils_NS::to_upper(const std::string& s) noexcept {
    auto result = s;
    std::transform(ALL_CONTAINER(result), result.begin(), [](unsigned char c) { return std::toupper(c); } );
    return result;
}

std::set<std::string> Utils_NS::getAllVariants(const std::string& data) noexcept {
    std::set<std::string> result;

    result.insert(data);
    result.insert(replace(data, '_', ' '));
    result.insert(replace(data, '_', '-'));
    result.insert(replace(data, '_', std::nullopt));

    return result;
}

std::vector<std::string> Utils_NS::split(const std::string& data, const char& c) noexcept {
    return {};
}

std::string Utils_NS::join(const std::vector<std::string>& data, const char& c) noexcept {
    return "";
}

void Utils_NS::prepend_inplace(std::vector<std::string>& data, const std::string& s) noexcept {
    for (auto& item : data)
        item = s + item;
}

std::vector<std::string> Utils_NS::prepend(const std::vector<std::string>& data, const std::string& s) noexcept {
    auto result = data;
    prepend_inplace(result, s);
    return result;
}
