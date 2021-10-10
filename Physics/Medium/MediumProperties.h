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

        /// Medium absorption coefficient at Temperature = 0, Degradation = 0
        std::optional<T> a0 = std::nullopt;
        /// d(Medium absorption coefficient)/d(Temperature)
        std::optional<T> aT = std::nullopt;
        /// d(Medium absorption coefficient)/d(Degradation)
        std::optional<T> aD = std::nullopt;
        /// d2(Medium absorption coefficient)/d(Degradation)d(Temperature). Will be temporarily used for arbitrary medium
        std::optional<T> aDT = std::nullopt;

        /// Medium scattering coefficient at Temperature = 0, Degradation = 0
        std::optional<T> u0 = std::nullopt;
        /// d(Medium scattering coefficient)/d(Temperature)
        std::optional<T> uT = std::nullopt;
        /// d(Medium scattering coefficient)/d(Degradation)
        std::optional<T> uD = std::nullopt;
        /// d2(Medium scattering coefficient)/d(Degradation)d(Temperature). Will be temporarily used for arbitrary medium
        std::optional<T> uDT = std::nullopt;

        /// Medium scattering anisotropy coefficient at Temperature = 0, Degradation = 0
        std::optional<T> g0 = std::nullopt;
        /// d(Medium scattering anisotropy coefficient)/d(Temperature)
        std::optional<T> gT = std::nullopt;
        /// d(Medium scattering anisotropy coefficient)/d(Degradation)
        std::optional<T> gD = std::nullopt;
        /// d2(Medium scattering anisotropy coefficient)/d(Degradation)d(Temperature). Will be temporarily used for arbitrary medium
        std::optional<T> gDT = std::nullopt;
    };
}
