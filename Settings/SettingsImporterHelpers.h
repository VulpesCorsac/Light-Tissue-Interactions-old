#pragma once

#include "SettingsImporterErrors.h"
#include "SettingsStrings.h"

#include "../yaml-cpp/include/yaml-cpp/yaml.h"

#include <stdexcept>

namespace Settings_NS {
    /// Get node with actual data
    /// \param[in] node yaml node to parse
    /// \return node with actual data
    inline YAML::Node valueNode(const YAML::Node& node);

    /// just as a sample
    inline void readFromYaml(bool& flag, const std::string& path);
}


/******************
 * IMPLEMENTATION *
 ******************/

YAML::Node Settings_NS::valueNode(const YAML::Node& node) {
    if (node.Type() == YAML::NodeType::Scalar)
        return node;
    if (const auto valueNode = node[SettingsStrings::Yaml::Value])
        return valueNode;
    return node;
}

void Settings_NS::readFromYaml(bool& flag, const std::string& path) {
    auto config = YAML::LoadFile(path.c_str());

    const std::string SettingsExample    = "SettingsExample";
    const std::string FeatureFlagEnabled = "FeatureFlagEnabled";

    if (const auto settingsExampleNode = config[SettingsExample]) {
        if (const auto featureFlagEnableNode = settingsExampleNode[FeatureFlagEnabled]) {
            const std::string description = featureFlagEnableNode[SettingsStrings::Yaml::Description] ?
                                            featureFlagEnableNode[SettingsStrings::Yaml::Description].as<std::string>() :
                                            "";
            if (const auto valueNode = featureFlagEnableNode[SettingsStrings::Yaml::Value])
                flag = valueNode.as<int>();
            else
                throwParsingErrorNoNode(SettingsExample + "/" + FeatureFlagEnabled + "/" + SettingsStrings::Yaml::Value, description);
        } else
            throwParsingErrorNoNode(SettingsExample + "/" + FeatureFlagEnabled);
    } else
        throwParsingErrorNoNode(SettingsExample);
}


