#pragma once

#include "SettingsImporterMedium.h"
#include "SettingsStrings.h"

#include "../Physics/Medium/MediumProperties.h"
#include "../Physics/Medium/MediumType.h"
#include "../Physics/Medium/MediumUtils.h"

#include "../yaml-cpp/include/yaml-cpp/yaml.h"

namespace Settings_NS {
    /// Returns yaml node based on MediumType
    /// \param[in] node yaml node to parse
    /// \return MediumType from the given node
    /// \throw std::invalid_argument throws same exception as Physics_NS::mediumType(string)
    YAML::Node mediumType(const Physics_NS::MediumType& type);

    template < typename T >
    YAML::Node mediumProperties(const Physics_NS::MediumProperties<T>& properties);
}

/******************
 * IMPLEMENTATION *
 ******************/

YAML::Node Settings_NS::mediumType(const Physics_NS::MediumType& type) {
    YAML::Node result;

    result[SettingsStrings::Medium::Type] = Physics_NS::to_string(type);

    return result;
}
