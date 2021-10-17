#pragma once

#include "SettingsImporterErrors.h"
#include "SettingsImporterStrings.h"

#include "../yaml-cpp/include/yaml-cpp/yaml.h"

#include <stdexcept>

namespace Settings_NS {
    void readFromYaml(bool& flag, const std::string& path) {
        auto config = YAML::LoadFile(path.c_str());

        const std::string SettingsExample    = "SettingsExample";
        const std::string FeatureFlagEnabled = "FeatureFlagEnabled";

        if (const auto settingsExampleNode = config[SettingsExample]) {
            if (const auto featureFlagEnableNode = settingsExampleNode[FeatureFlagEnabled]) {
                const std::string description = featureFlagEnableNode[Settings_NS::SettingsStrings::StringDescription] ?
                                                featureFlagEnableNode[Settings_NS::SettingsStrings::StringDescription].as<std::string>() :
                                                "";
                if (const auto valueNode = featureFlagEnableNode[Settings_NS::SettingsStrings::StringValue])
                    flag = valueNode.as<int>();
                else
                    throwParsingErrorNoNode(SettingsExample + "/" + FeatureFlagEnabled + "/" + Settings_NS::SettingsStrings::StringValue, description);
            } else
                throwParsingErrorNoNode(SettingsExample + "/" + FeatureFlagEnabled);
        } else
            throwParsingErrorNoNode(SettingsExample);
    }

}
