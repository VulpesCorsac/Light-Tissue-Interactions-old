#pragma once

#include "DetectorType.h"

#include <optional>

namespace MonteCarlo_NS {
    /// \brief Full properties for Detector that is not arbitrary type
    template < typename T >
    struct DetectorProperties {
        /// Detector type
        DetectorType type = DetectorType::Unknown;

        /// cosine for photon to be evaluated as collimated
        std::optional<T> collimatedCosine = std::nullopt;
    };
}
