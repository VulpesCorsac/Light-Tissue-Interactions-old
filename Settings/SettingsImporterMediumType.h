#pragma once

#include "SettingsImporterErrors.h"
#include "SettingsImporterStrings.h"

#include "../Physics/Medium/MediumType.h"
#include "../Physics/Medium/MediumUtils.h"

#include "../yaml-cpp/include/yaml-cpp/yaml.h"

#ifdef ASSERT_INPUT_PARAMS
    #include <stdexcept>
#endif // ASSERT_INPUT_PARAMS

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
    if (node.Type() == YAML::NodeType::Null) {
        #ifdef ASSERT_INPUT_PARAMS
            throw std::invalid_argument("Node is empty thus cannot evaluate medium type");
        #endif // ASSERT_INPUT_PARAMS

        return Physics_NS::MediumType::Unknown;
    }

    if (node.Type() == YAML::NodeType::Scalar)
        return Physics_NS::mediumType(node.as<std::string>());

    if (const auto valueNode = node[StringValue])
        return Physics_NS::mediumType(valueNode.as<std::string>());

    #ifdef ASSERT_INPUT_PARAMS
        throw std::invalid_argument("Not any branch was used for medium evaluation");
    #endif // ASSERT_INPUT_PARAMS

    return Physics_NS::MediumType::Unknown;
}
