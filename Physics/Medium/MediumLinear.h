#pragma once

#include "MediumInterface.h"
#include "MediumProperties.h"
#include "MediumType.h"

namespace Physics_NS {
    /// \brief class for linear Medium
    template < typename T >
    class MediumLinear : public MediumInterface<T> {
    public:
        /// \param[in] n medium refraction index
        explicit MediumLinear(const T& n = 1) noexcept;
        /// \param[in] properties linear medium MediumProperties
        explicit MediumLinear(const MediumProperties<T>& properties) noexcept;
    };
}

/******************
 * IMPLEMENTATION *
 ******************/

template < typename T >
Physics_NS::MediumLinear<T>::MediumLinear(const T& n) noexcept
    : MediumInterface<T>(MediumType::Linear) {
}

template < typename T >
Physics_NS::MediumLinear<T>::MediumLinear(const MediumProperties<T>& properties) noexcept
    : MediumInterface<T>(MediumType::Linear) {
}
