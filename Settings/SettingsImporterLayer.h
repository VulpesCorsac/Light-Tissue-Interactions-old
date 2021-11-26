#pragma once

#include "SettingsImporterErrors.h"
#include "SettingsImporterHelpers.h"
#include "SettingsImporterMedium.h"
#include "SettingsStrings.h"

#include "../Physics/Layer.h"
#include "../Physics/Medium/MediumFactory.h"
#include "../Physics/Medium/MediumUtils.h"

#include "../yaml-cpp/include/yaml-cpp/yaml.h"

#include <memory>
#include <optional>

namespace Settings_NS {
    /// Returns Layer from yaml node
    /// \param[in] node yaml node to parse
    /// \return Layer from the given node
    /// \throw std::invalid_argument throws same exception as mediumProperties(const YAML::Node& node)
    /// \throw std::logic_error      throws same exception as mediumProperties(const YAML::Node& node)
    /// \throw std::logic_error      throws in case property was set twice or any other error happens
    template < typename T >
    Physics_NS::Layer<T> layerFromYaml(const YAML::Node& node);
}

/******************
 * IMPLEMENTATION *
 ******************/

template < typename T >
Physics_NS::Layer<T> Settings_NS::layerFromYaml(const YAML::Node& node) {
    std::optional<T> begin = std::nullopt;
    std::optional<T> end   = std::nullopt;
    std::optional<T> width = std::nullopt;
    std::optional<Physics_NS::MediumProperties<T>> properties = std::nullopt;

    for (const auto it: valueNode(node)) {
        const auto key = it.first.as<std::string>();
        const auto value = it.second;
        if (key == Settings_NS::SettingsStrings::Layer::Medium) {
            if (properties.has_value())
                throw std::logic_error("Trying to set medium type twice");
            else
                properties = Settings_NS::mediumProperties<T>(value);
        } else {
            auto helper = [&key, &value]
                          (const std::string& propertyString, std::optional<T>& propertyValue) -> void {
                if (key == propertyString) {
                    const auto val = valueNode(value);
                    if (val.Type() != YAML::NodeType::Scalar)
                        throw std::logic_error("Layer " + propertyString + ": value must be scalar");
                    if (!propertyValue.has_value())
                        propertyValue = val.as<T>();
                    else
                        throw std::logic_error("Trying to set layer " + propertyString + " twice");
                }
            };

            helper(SettingsStrings::Layer::Begin, begin);
            helper(SettingsStrings::Layer::End  , end  );
            helper(SettingsStrings::Layer::Width, width);
        }
    }

    if (!width.has_value() && !begin.has_value() && !end.has_value())
        throw std::logic_error("Layer doesn't have width, begin and end");

    if (!width.has_value() && (!begin.has_value() && end.has_value()) || (begin.has_value() && !end.has_value()))
        throw std::logic_error("Layer doesn't have width and only has begin or end");

    if (!properties.has_value())
        throw std::logic_error("Layer doesn't have mediumProperties");

    if (width.has_value())
        return Physics_NS::Layer<T>(Physics_NS::createMedium(properties.value()), width.value());
    return Physics_NS::Layer<T>(Physics_NS::createMedium(properties.value()), begin.value(), end.value());
}
