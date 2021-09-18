#pragma once

#include "MediumType.h"

namespace Physics_NS {
    /// \brief Full properties for Medium that is not arbitrary type
    template < typename T >
    struct MediumProperties {
        /// Medium type
        MediumType type = MediumType::Unknown;

        /// Medium refraction index at Temperature = 0, Degradation = 0
        T n0 = 0;
        /// d(Medium refraction index)/d(Temperature)
        T nT = 0;
        /// d(Medium refraction index)/d(Degradation)
        T nD = 0;
    };

}
