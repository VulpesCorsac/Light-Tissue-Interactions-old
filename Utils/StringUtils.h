#pragma once

#include "Utils.h"

#include <set>

namespace Utils_NS {
    /// change string with replaced characters
    /// \param[in,out] s string to replace characters in
    /// \param[in] from character to replace from
    /// \param[in] to replace found "from" to "to" if needed
    inline void replace_inplace(std::string& s, char from, std::optional<char> to);
    /// returns string with replaced characters
    /// \param[in] s string to replace characters in
    /// \param[in] from character to replace from
    /// \param[in] to replace found "from" to "to" if needed
    /// \return std::string result after replacing
    inline std::string replace(const std::string& s, char from, std::optional<char> to);

    /// change string with all characters changed to lower case
    /// \param[in,out] s original string
    inline void to_lower_inplace(std::string& s);
    /// returns string with all characters changed to lower case
    /// \param[in] s original string
    /// \return std::string result with all characters changed to lower case
    inline std::string to_lower(const std::string& s);
    /// change string with all characters changed to upper case
    /// \param[in,out] s original string
    inline void to_upper_inplace(std::string& s);
    /// returns string with all characters changed to upper case
    /// \param[in] s original string
    /// \return std::string result with all characters changed to upper case
    inline std::string to_upper(const std::string& s);

    /// returns replacement of all '_' symbols with '-', ' ' and simply deleting it
    /// \param[in] data original string
    /// \return std::set<std::string> result all replacements
    inline std::set<std::string> getAllVariants(const std::string& data);

    /// returns string splitted by delimeter
    /// \param[in] data original string
    /// \param[in] delim delimeter to split
    /// \return std::vector<std::string> result of splitting
    inline std::vector<std::string> split(const std::string& data, char delim);
    /// returns string splitted by delimeter
    /// \param[in] data original string
    /// \param[in] delim delimeter to split
    /// \return std::vector<std::string> result of splitting
    inline std::vector<std::string> split(const std::string& data, const std::string& delim);

    /// returns string joined with c delimeter
    /// \param[in] data original data
    /// \param[in] delim to insert between string in data
    /// \param[in] endWithDelim put delim at the end of the result
    /// \return std::string joined string
    inline std::string join(const std::vector<std::string>& data, const std::string& delim, bool endWithDelim = false);

    /// deletes all empty string from the original data
    /// \param[in,out] data original data
    inline void remove_empty_inplace(std::vector<std::string>& data);
    /// returns copy of original vector with all empty strings removed
    /// \param[in] data original data
    /// \return std::vector<std::string> result of removal
    inline std::vector<std::string> remove_empty(const std::vector<std::string>& data);

    /// prepends data with s
    /// \param[in,out] data original string
    /// \param[in] s string to prepend
    inline void prepend_inplace(std::string& data, const std::string& s);
    /// returns data with prepended s
    /// \param[in] data original string
    /// \param[in] s string to prepend
    /// \return std::string data prepended with prepended s
    inline std::string prepend(const std::string& data, const std::string& s);
    /// prepends all strings in data with prepended s
    /// \param[in,out] data original string vector
    /// \param[in] s string to prepend
    inline void prepend_inplace(std::vector<std::string>& data, const std::string& s);
    /// returns all strings in data with prepended s
    /// \param[in] data original string vector
    /// \param[in] s string to prepend
    /// \return std::vector<std::string> copy of all strings in data with prepended s
    inline std::vector<std::string> prepend(const std::vector<std::string>& data, const std::string& s);
}

/******************
 * IMPLEMENTATION *
 ******************/

void Utils_NS::replace_inplace(std::string& s, char from, std::optional<char> to) {
    s = replace(s, from, to);
}

std::string Utils_NS::replace(const std::string& s, char from, std::optional<char> to) {
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

void Utils_NS::to_lower_inplace(std::string& s) {
    std::transform(ALL_CONTAINER(s), s.begin(), [](unsigned char c) { return std::tolower(c); } );
}

std::string Utils_NS::to_lower(const std::string& s) {
    auto result = s;
    std::transform(ALL_CONTAINER(result), result.begin(), [](unsigned char c) { return std::tolower(c); } );
    return result;
}

void Utils_NS::to_upper_inplace(std::string& s) {
    std::transform(ALL_CONTAINER(s), s.begin(), [](unsigned char c) { return std::toupper(c); } );
}

std::string Utils_NS::to_upper(const std::string& s) {
    auto result = s;
    std::transform(ALL_CONTAINER(result), result.begin(), [](unsigned char c) { return std::toupper(c); } );
    return result;
}

std::set<std::string> Utils_NS::getAllVariants(const std::string& data) {
    std::set<std::string> result;

    result.insert(data);
    result.insert(replace(data, '_', ' '));
    result.insert(replace(data, '_', '-'));
    result.insert(replace(data, '_', std::nullopt));

    return result;
}

std::vector<std::string> Utils_NS::split(const std::string& data, char delim) {
    return split(data, std::string() + delim);
}

std::vector<std::string> Utils_NS::split(const std::string& data, const std::string& delim) {
    std::vector<std::string> result;

    if (!data.empty()) {
        auto start = 0U;
        auto end = data.find(delim);
        while (end != std::string::npos) {
            result.emplace_back(data.substr(start, end - start));
            start = end + delim.length();
            end = data.find(delim, start);
        }

        result.emplace_back(data.substr(start, end));
    }

    return result;
}

std::string Utils_NS::join(const std::vector<std::string>& data, const std::string& delim, bool endWithDelim) {
    std::string result;

    for (int i = 0; i < isize(data); ++i)
        result += i ? delim + data[i] : data[i];

    if (endWithDelim)
        result += delim;

    return result;
}

void Utils_NS::remove_empty_inplace(std::vector<std::string>& data) {
    data = remove_empty(data);
}

std::vector<std::string> Utils_NS::remove_empty(const std::vector<std::string>& data) {
    std::vector<std::string> result;

    for (const auto& item: data)
        if (!item.empty())
            result.push_back(item);

    return result;
}

void Utils_NS::prepend_inplace(std::string& data, const std::string& s) {
    data = prepend(data, s);
}

std::string Utils_NS::prepend(const std::string& data, const std::string& s) {
    return s + data;
}

void Utils_NS::prepend_inplace(std::vector<std::string>& data, const std::string& s) {
    for (auto& item : data)
        prepend_inplace(item, s);
}

std::vector<std::string> Utils_NS::prepend(const std::vector<std::string>& data, const std::string& s) {
    auto result = data;
    prepend_inplace(result, s);
    return result;
}
