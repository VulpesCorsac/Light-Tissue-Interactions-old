#pragma once

#include <yaml-cpp/yaml.h>

void readFromYaml(bool& flag) {
    YAML::Node config = YAML::LoadFile("Settings/settings.yaml");

    if (config["Settings"])
        if (config["Settings"]["FeatureFlagEnabled"])
            flag = config["Settings"]["FeatureFlagEnabled"].as<int>();
}
