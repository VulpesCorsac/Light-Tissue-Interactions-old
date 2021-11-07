#pragma once

#include "SettingsExporterMedium.h"
#include "SettingsStrings.h"

#include "../Physics/Layer.h"
#include "../Physics/Medium/MediumUtils.h"

#include "../yaml-cpp/include/yaml-cpp/yaml.h"

namespace Settings_NS {
    /// Returns yaml node based on Layer
    /// \param[in] Layer layer
    /// \return yaml node with serialized layer
    template < typename T >
    YAML::Node layerToyaml(const Physics_NS::Layer<T>& layer);
}

/******************
 * IMPLEMENTATION *
 ******************/

template < typename T >
YAML::Node Settings_NS::layerToyaml(const Physics_NS::Layer<T>& layer) {
    YAML::Node result;

    result[SettingsStrings::Layer::Begin] = layer.begin;
    result[SettingsStrings::Layer::End  ] = layer.end  ;
    result[SettingsStrings::Layer::Width] = layer.width;

    result[SettingsStrings::Layer::Medium] = mediumProperties(exportMediumProperties(layer.medium.get()));

    return result;
}
