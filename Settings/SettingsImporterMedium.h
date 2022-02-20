#pragma once

#include "SettingsImporterErrors.h"
#include "SettingsImporterHelpers.h"
#include "SettingsStrings.h"

#include "../Physics/Medium/MediumProperties.h"
#include "../Physics/Medium/MediumType.h"
#include "../Physics/Medium/MediumUtils.h"

#include "../Utils/Contracts.h"

#include "../yaml-cpp/include/yaml-cpp/yaml.h"

namespace Settings_NS {
    /// Returns MediumType from yaml node
    /// \param[in] node yaml node to parse
    /// \return MediumType from the given node
    /// \throw std::invalid_argument throws same exception as Physics_NS::mediumType(string)
    static Physics_NS::MediumType mediumType(const YAML::Node& node) EXCEPT_INPUT_PARAMS;

    /// Returns MediumProperties from yaml node
    /// \param[in] node yaml node to parse
    /// \return MediumProperties from the given node
    /// \throw std::invalid_argument throws same exception as Physics_NS::mediumType(string)
    /// \throw std::logic_error      throws same exception as Physics_NS::validate(const Physics_NS::MediumProperties<T>&)
    /// \throw std::logic_error      throws in case property was set twice or any other error happens
    template < typename T >
    Physics_NS::MediumProperties<T> mediumProperties(const YAML::Node& node);
}

/******************
 * IMPLEMENTATION *
 ******************/

Physics_NS::MediumType Settings_NS::mediumType(const YAML::Node& node) EXCEPT_INPUT_PARAMS {
    if (node.Type() == YAML::NodeType::Null) {
        FAIL_ARGUMENT_CONTRACT("Node is empty thus cannot evaluate medium type");

        return Physics_NS::MediumType::Unknown;
    }

    if (node.Type() == YAML::NodeType::Scalar)
        return Physics_NS::mediumType(node.as<std::string>());

    if (const auto valueNode = node[Settings_NS::SettingsStrings::Yaml::Value])
        return Physics_NS::mediumType(valueNode.as<std::string>());

    if (const auto value = node[Settings_NS::SettingsStrings::Medium::Type])
        return Physics_NS::mediumType(value.as<std::string>());

    FAIL_ARGUMENT_CONTRACT("Not any branch was used for medium evaluation");

    return Physics_NS::MediumType::Unknown;
}

template < typename T >
Physics_NS::MediumProperties<T> Settings_NS::mediumProperties(const YAML::Node& node) {
    Physics_NS::MediumProperties<T> result;

    for (const auto it: valueNode(node)) {
        const auto key = it.first.as<std::string>();
        const auto value = it.second;
        if (key == Settings_NS::SettingsStrings::Medium::Type) {
            if (result.type == Physics_NS::MediumType::Unknown)
                result.type = mediumType(value);
            else
                throw std::logic_error("Trying to set medium type twice");
        } else {
            auto helper = [&key, &value]
                          (const std::string &propertyString, std::optional<T>& propertyValue) -> void {
                if (key == propertyString) {
                    const auto val = valueNode(value);
                    if (val.Type() != YAML::NodeType::Scalar)
                        throw std::logic_error("Medium " + propertyString + ": value must be scalar");
                    if (!propertyValue.has_value())
                        propertyValue = val.as<T>();
                    else
                        throw std::logic_error("Trying to set medium " + propertyString + " twice");
                }
            };

            helper(Settings_NS::SettingsStrings::Medium::N0 , result.n0 );
            helper(Settings_NS::SettingsStrings::Medium::NT , result.nT );
            helper(Settings_NS::SettingsStrings::Medium::ND , result.nD );
            helper(Settings_NS::SettingsStrings::Medium::NDT, result.nDT);
            helper(Settings_NS::SettingsStrings::Medium::A0 , result.a0 );
            helper(Settings_NS::SettingsStrings::Medium::AT , result.aT );
            helper(Settings_NS::SettingsStrings::Medium::AD , result.aD );
            helper(Settings_NS::SettingsStrings::Medium::ADT, result.aDT);
            helper(Settings_NS::SettingsStrings::Medium::U0 , result.u0 );
            helper(Settings_NS::SettingsStrings::Medium::UT , result.uT );
            helper(Settings_NS::SettingsStrings::Medium::UD , result.uD );
            helper(Settings_NS::SettingsStrings::Medium::UDT, result.uDT);
            helper(Settings_NS::SettingsStrings::Medium::G0 , result.g0 );
            helper(Settings_NS::SettingsStrings::Medium::GT , result.gT );
            helper(Settings_NS::SettingsStrings::Medium::GD , result.gD );
            helper(Settings_NS::SettingsStrings::Medium::GDT, result.gDT);
            helper(Settings_NS::SettingsStrings::Medium::R0 , result.r0 );
            helper(Settings_NS::SettingsStrings::Medium::RT , result.rT );
            helper(Settings_NS::SettingsStrings::Medium::RD , result.rD );
            helper(Settings_NS::SettingsStrings::Medium::RDT, result.rDT);
            helper(Settings_NS::SettingsStrings::Medium::C0 , result.c0 );
            helper(Settings_NS::SettingsStrings::Medium::CT , result.cT );
            helper(Settings_NS::SettingsStrings::Medium::CD , result.cD );
            helper(Settings_NS::SettingsStrings::Medium::CDT, result.cDT);
            helper(Settings_NS::SettingsStrings::Medium::K0 , result.k0 );
            helper(Settings_NS::SettingsStrings::Medium::KT , result.kT );
            helper(Settings_NS::SettingsStrings::Medium::KD , result.kD );
            helper(Settings_NS::SettingsStrings::Medium::KDT, result.kDT);
        }
    }

    return result;
}
