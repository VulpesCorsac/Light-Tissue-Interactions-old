#pragma once

#include "MediumType.h"

#include <optional>

namespace Physics_NS {
    /// \brief Full properties for Medium that is not arbitrary type
    template < typename T >
    struct MediumProperties {
        /// Medium type
        MediumType type = MediumType::Unknown;

        /// Medium refraction index at Temperature = 0, Degradation = 0
        std::optional<T> n0 = std::nullopt;
        /// d(Medium refraction index)/d(Temperature)
        std::optional<T> nT = std::nullopt;
        /// d(Medium refraction index)/d(Degradation)
        std::optional<T> nD = std::nullopt;
        /// d2(Medium refraction index)/d(Degradation)d(Temperature). Will be temporarily used for arbitrary medium
        std::optional<T> nDT = std::nullopt;
    };

}
