#pragma once

#include "MediumInterface.h"
#include "MediumProperties.h"
#include "MediumType.h"

namespace Physics_NS {
    /// \brief class for constant Medium
    template < typename T >
    class MediumConstant : public MediumInterface<T> {
    public:
        /// \param[in] n medium refraction index
        explicit MediumConstant(const T& n = 1) noexcept;
        /// \param[in] properties glass MediumProperties
        explicit MediumConstant(const MediumProperties<T>& properties) noexcept;
    };
}

/******************
 * IMPLEMENTATION *
 ******************/

template < typename T >
Physics_NS::MediumConstant<T>::MediumConstant(const T& n) noexcept
    : MediumInterface<T>(MediumType::Constant) {
}

template < typename T >
Physics_NS::MediumConstant<T>::MediumConstant(const MediumProperties<T>& properties) noexcept
    : MediumInterface<T>(MediumType::Constant) {
}
