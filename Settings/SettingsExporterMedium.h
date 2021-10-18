#pragma once

#include "SettingsStrings.h"

#include "../Physics/Medium/MediumProperties.h"
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
    YAML::Node mediumType(const Physics_NS::MediumType& type);
}

/******************
 * IMPLEMENTATION *
 ******************/

YAML::Node Settings_NS::mediumType(const Physics_NS::MediumType& type) {
    YAML::Node result;

    result[SettingsStrings::Medium::Type] = Physics_NS::to_string(type);

    return result;
}
