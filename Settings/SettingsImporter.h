#pragma once

#include "../yaml-cpp/include/yaml-cpp/yaml.h"

#include <stdexcept>

void throwParsingErrorNoNode(const std::string& path, const std::string& description = "") {
    const std::string descriptionToShow = description.empty() ? "No description provided" : description;
    throw std::runtime_error("Error in parsing yaml in node: " + path +
                             ", no such node, node description: " + descriptionToShow);
}

void readFromYaml(bool& flag, const std::string& path) {
    auto config = YAML::LoadFile(path.c_str());

    const std::string Value              = "value";
    const std::string Description        = "description";
    const std::string SettingsExample    = "SettingsExample";
    const std::string FeatureFlagEnabled = "FeatureFlagEnabled";

    if (const auto settingsExampleNode = config[SettingsExample]) {
        if (const auto featureFlagEnableNode = settingsExampleNode[FeatureFlagEnabled]) {
            const std::string description = featureFlagEnableNode[Description] ? featureFlagEnableNode[Description].as<std::string>() : "";
            if (const auto valueNode = featureFlagEnableNode[Value]) {
                flag = valueNode.as<int>();
            } else
                throwParsingErrorNoNode(SettingsExample + "/" + FeatureFlagEnabled + "/" + Value, description);
        } else
            throwParsingErrorNoNode(SettingsExample + "/" + FeatureFlagEnabled);
    } else
        throwParsingErrorNoNode(SettingsExample);
}
