#pragma once

#include "MediumInterface.h"
#include "MediumProperties.h"
#include "MediumType.h"

namespace Physics_NS {
    /// \brief class for arbitrary Medium
    template < typename T >
    class MediumArbitrary : public MediumInterface<T> {
    public:
        /// \param[in] n medium refraction index
        explicit MediumArbitrary(const T& n = 1) noexcept;
        /// \param[in] properties arbitrary medium MediumProperties
        explicit MediumArbitrary(const MediumProperties<T>& properties) noexcept;
    };
}

/******************
 * IMPLEMENTATION *
 ******************/

template < typename T >
Physics_NS::MediumArbitrary<T>::MediumArbitrary(const T& n) noexcept
    : MediumInterface<T>(MediumType::Arbitrary) {
}

template < typename T >
Physics_NS::MediumArbitrary<T>::MediumArbitrary(const MediumProperties<T>& properties) noexcept
    : MediumInterface<T>(MediumType::Arbitrary) {
    std::ignore = properties;
}
