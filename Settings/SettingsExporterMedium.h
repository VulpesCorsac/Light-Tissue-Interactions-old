#pragma once

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
    using namespace Physics_NS;

    YAML::Node result;

    result[SettingsStrings::Medium::Type] = to_string(type);

    return result;
}

template < typename T >
YAML::Node Settings_NS::mediumProperties(const Physics_NS::MediumProperties<T>& properties) {
    using namespace Physics_NS;

    YAML::Node result;

    result[SettingsStrings::Medium::Type] = to_string(properties.type);

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

    if (properties.r0.has_value())
        result[SettingsStrings::Medium::R0 ] = std::to_string(properties.r0.value());
    if (properties.rT.has_value())
        result[SettingsStrings::Medium::RT ] = std::to_string(properties.rT.value());
    if (properties.rD.has_value())
        result[SettingsStrings::Medium::RD ] = std::to_string(properties.rD.value());
    if (properties.rDT.has_value())
        result[SettingsStrings::Medium::RDT] = std::to_string(properties.rDT.value());

    if (properties.c0.has_value())
        result[SettingsStrings::Medium::C0 ] = std::to_string(properties.c0.value());
    if (properties.cT.has_value())
        result[SettingsStrings::Medium::CT ] = std::to_string(properties.cT.value());
    if (properties.cD.has_value())
        result[SettingsStrings::Medium::CD ] = std::to_string(properties.cD.value());
    if (properties.cDT.has_value())
        result[SettingsStrings::Medium::CDT] = std::to_string(properties.cDT.value());

    if (properties.k0.has_value())
        result[SettingsStrings::Medium::K0 ] = std::to_string(properties.k0.value());
    if (properties.kT.has_value())
        result[SettingsStrings::Medium::KT ] = std::to_string(properties.kT.value());
    if (properties.kD.has_value())
        result[SettingsStrings::Medium::KD ] = std::to_string(properties.kD.value());
    if (properties.kDT.has_value())
        result[SettingsStrings::Medium::KDT] = std::to_string(properties.kDT.value());

    return result;
}
