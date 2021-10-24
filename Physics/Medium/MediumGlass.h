#pragma once

#include "MediumInterface.h"
#include "MediumProperties.h"
#include "MediumType.h"

namespace Physics_NS {
    /// \brief class for glass Medium
    template < typename T >
    class MediumGlass : public MediumInterface<T> {
    public:
        using Base = MediumInterface<T>;

        /// \param[in] n glass refraction index
        /// \param[in] r glass density
        /// \param[in] c glass heat capacity
        /// \param[in] k glass thermal conductivity
        explicit MediumGlass(const T& n = 1,
                             const T& r = 0,
                             const T& c = 0,
                             const T& k = 0) noexcept;
        /// \param[in] properties glass MediumProperties
        explicit MediumGlass(const MediumProperties<T>& properties) noexcept;

        T refraction          (const T& t = 0, const T& d = 0) const override;
        T density             (const T& t = 0, const T& d = 0) const override;
        T heat_capacity       (const T& t = 0, const T& d = 0) const override;
        T thermal_conductivity(const T& t = 0, const T& d = 0) const override;

    protected:
        const T n0; ///< Glass refraction index
        const T r0; ///< Glass density
        const T c0; ///< Glass heat capacity
        const T k0; ///< Glass thermal conductivity
    };
}

/******************
 * IMPLEMENTATION *
 ******************/

template < typename T >
Physics_NS::MediumGlass<T>::MediumGlass(const T& n, const T& r, const T& c, const T& k) noexcept
    : Physics_NS::MediumGlass<T>::Base(MediumType::Glass)
    , n0(n)
    , r0(r)
    , c0(c)
    , k0(k) {
}

template < typename T >
Physics_NS::MediumGlass<T>::MediumGlass(const MediumProperties<T>& properties) noexcept
    : Physics_NS::MediumGlass<T>::Base(MediumType::Glass)
    , n0(properties.n0.has_value() ? properties.n0.value() : 1)
    , r0(properties.r0.has_value() ? properties.r0.value() : 0)
    , c0(properties.c0.has_value() ? properties.c0.value() : 0)
    , k0(properties.k0.has_value() ? properties.k0.value() : 0)  {
}

template < typename T >
T Physics_NS::MediumGlass<T>::refraction(const T& t, const T& d) const {
    std::ignore = t;
    std::ignore = d;
    return n0;
}

template < typename T >
T Physics_NS::MediumGlass<T>::density(const T& t, const T& d) const {
    std::ignore = t;
    std::ignore = d;
    return r0;
}

template < typename T >
T Physics_NS::MediumGlass<T>::heat_capacity(const T& t, const T& d) const {
    std::ignore = t;
    std::ignore = d;
    return c0;
}

template < typename T >
T Physics_NS::MediumGlass<T>::thermal_conductivity(const T& t, const T& d) const {
    std::ignore = t;
    std::ignore = d;
    return k0;
}
