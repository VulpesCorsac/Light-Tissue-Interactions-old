#pragma once

#include "../yaml-cpp/include/yaml-cpp/yaml.h"

#include <sstream>
#include <string>

namespace Settings_NS {
    /// convert node to string
    /// \param[in] node YAML::Node to convert to string
    /// \return string representing node
    inline std::string to_string(const YAML::Node& node);
}

/******************
 * IMPLEMENTATION *
 ******************/

std::string Settings_NS::to_string(const YAML::Node& node) {
    std::stringstream ss;
    ss << node;
    return ss.str();
}
