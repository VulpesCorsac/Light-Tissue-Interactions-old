#pragma once

#include <stdexcept>
#include <string>

namespace Settings_NS {
    /// Function throws exception with useful message
    /// \param[in] path yaml nodes path
    /// \param[in] description yaml node description
    /// \throw std::runtime_error throws exception
    inline void throwParsingErrorNoNode(const std::string& path, const std::string& description = "");
}

/******************
 * IMPLEMENTATION *
 ******************/

void Settings_NS::throwParsingErrorNoNode(const std::string& path, const std::string& description) {
    using namespace std;

    const string descriptionToShow = description.empty() ? "No description provided" : description;
    throw runtime_error("Error in parsing yaml in node: " + path + ", no such node, node description: " + descriptionToShow);
}
