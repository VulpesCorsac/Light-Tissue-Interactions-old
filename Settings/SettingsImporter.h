#pragma once

#include <yaml-cpp/yaml.h>

#include <stdexcept>

void throwParsingErrorNoNode(const std::string& s) {
    throw std::runtime_error("Error in parsing yaml in node: " + s + ", no such node");
}

void readFromYaml(bool& flag, const std::string& path) {
    auto config = YAML::LoadFile(path.c_str());

    const std::string Value              = "value";
    const std::string Description        = "description";
    const std::string SettingsExample    = "SettingsExample";
    const std::string FeatureFlagEnabled = "FeatureFlagEnabled";

    if (auto settingsExampleNode = config[SettingsExample]) {
        if (auto featureFlagEnableNode = settingsExampleNode[FeatureFlagEnabled]) {
            if (auto valueNode = featureFlagEnableNode[Value]) {
                flag = valueNode.as<int>();
            } else
                throwParsingErrorNoNode(SettingsExample + "/" + FeatureFlagEnabled + "/" + Value);
        } else
            throwParsingErrorNoNode(SettingsExample + "/" + FeatureFlagEnabled);
    } else
        throwParsingErrorNoNode(SettingsExample);
}
