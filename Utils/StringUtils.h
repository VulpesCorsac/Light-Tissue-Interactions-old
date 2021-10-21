#pragma once

#include "Utils.h"

#include <set>

namespace Utils_NS {
    inline std::set<std::string> getAllVariants(const std::string& s) noexcept;
}

/******************
 * IMPLEMENTATION *
 ******************/

std::set<std::string> Utils_NS::getAllVariants(const std::string& s) noexcept {
    std::set<std::string> result;

    result.insert(s);
    result.insert(replace(s, '_', ' '));
    result.insert(replace(s, '_', '-'));
    result.insert(replace(s, '_', std::nullopt));

    return result;
}
