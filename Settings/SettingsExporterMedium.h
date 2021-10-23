#pragma once

#include "SettingsImporterMedium.h"
#include "SettingsStrings.h"

#include "../Physics/Medium/MediumProperties.h"
#include "../Physics/Medium/MediumType.h"
#include "../Physics/Medium/MediumUtils.h"

#include "../yaml-cpp/include/yaml-cpp/yaml.h"

namespace Settings_NS {
    /// Returns yaml node based on MediumType
    /// \param[in] MediumType medium type
    /// \return yaml node with serialized type
    inline YAML::Node mediumType(const Physics_NS::MediumType& type);

    /// Returns yaml node based on MediumProperties
    /// \param[in] MediumProperties medium properties
    /// \return yaml node with serialized properties
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

template < typename T >
YAML::Node Settings_NS::mediumProperties(const Physics_NS::MediumProperties<T>& properties) {
    YAML::Node result;

    result[SettingsStrings::Medium::Type] = Physics_NS::to_string(properties.type);

    if (properties.n0.has_value())
        result[SettingsStrings::Medium::N0 ] = std::to_string(properties.n0.value());
    if (properties.nT.has_value())
        result[SettingsStrings::Medium::NT ] = std::to_string(properties.nT.value());
    if (properties.nD.has_value())
        result[SettingsStrings::Medium::ND ] = std::to_string(properties.nD.value());
    if (properties.nDT.has_value())
        result[SettingsStrings::Medium::NDT] = std::to_string(properties.nDT.value());

    if (properties.a0.has_value())
        result[SettingsStrings::Medium::A0 ] = std::to_string(properties.a0.value());
    if (properties.aT.has_value())
        result[SettingsStrings::Medium::AT ] = std::to_string(properties.aT.value());
    if (properties.aD.has_value())
        result[SettingsStrings::Medium::AD ] = std::to_string(properties.aD.value());
    if (properties.aDT.has_value())
        result[SettingsStrings::Medium::ADT] = std::to_string(properties.aDT.value());

    if (properties.u0.has_value())
        result[SettingsStrings::Medium::U0 ] = std::to_string(properties.u0.value());
    if (properties.uT.has_value())
        result[SettingsStrings::Medium::UT ] = std::to_string(properties.uT.value());
    if (properties.uD.has_value())
        result[SettingsStrings::Medium::UD ] = std::to_string(properties.uD.value());
    if (properties.uDT.has_value())
        result[SettingsStrings::Medium::UDT] = std::to_string(properties.uDT.value());

    if (properties.g0.has_value())
        result[SettingsStrings::Medium::G0 ] = std::to_string(properties.g0.value());
    if (properties.gT.has_value())
        result[SettingsStrings::Medium::GT ] = std::to_string(properties.gT.value());
    if (properties.gD.has_value())
        result[SettingsStrings::Medium::GD ] = std::to_string(properties.gD.value());
    if (properties.gDT.has_value())
        result[SettingsStrings::Medium::GDT] = std::to_string(properties.gDT.value());

    return result;
}
