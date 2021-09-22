#pragma once

#include "SettingsImporterErrors.h"
#include "SettingsImporterStrings.h"

#include "../Physics/Medium/MediumType.h"
#include "../Physics/Medium/MediumUtils.h"

#include "../yaml-cpp/include/yaml-cpp/yaml.h"

namespace Settings_NS {
    /// Returns MediumType from yaml node
    /// \param[in] node yaml node to parse
    /// \return MediumType from the given node
    /// \throw std::invalid_argument throws same exception as Physics_NS::mediumType(string)
    Physics_NS::MediumType mediumType(const YAML::Node& node);
}

/******************
 * IMPLEMENTATION *
 ******************/

Physics_NS::MediumType Settings_NS::mediumType(const YAML::Node& node) {
    if (const auto valueNode = node[StringValue])
        return Physics_NS::mediumType(valueNode.as<std::string>());

    return Physics_NS::mediumType(node.as<std::string>());
}
