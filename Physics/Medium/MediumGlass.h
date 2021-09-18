#pragma once

#include "MediumInterface.h"
#include "MediumProperties.h"
#include "MediumType.h"

namespace Physics_NS {
    /// \brief class for glass Medium
    template < typename T >
    class MediumGlass : public MediumInterface<T> {
    public:
        /// \param[in] n glass refraction index
        explicit MediumGlass(const T& n = 1) noexcept;
        /// \param[in] properties glass MediumProperties
        explicit MediumGlass(const MediumProperties<T>& properties) noexcept;

        T refraction(const T& t = 0, const T& d = 0) const override;

    protected:
        const T n; ///< Glass refraction index
    };
}

/******************
 * IMPLEMENTATION *
 ******************/

template < typename T >
Physics_NS::MediumGlass<T>::MediumGlass(const T& n) noexcept
    : MediumInterface<T>(MediumType::Glass)
    , n(n) {
}

template < typename T >
Physics_NS::MediumGlass<T>::MediumGlass(const MediumProperties<T>& properties) noexcept
    : MediumInterface<T>(MediumType::Glass)
    , n(properties.n0) {
}

template < typename T >
T Physics_NS::MediumGlass<T>::refraction(const T& t, const T& d) const {
    return n;
}
