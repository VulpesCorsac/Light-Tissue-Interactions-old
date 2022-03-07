#pragma once

#include "MediumInterface.h"
#include "MediumProperties.h"
#include "MediumType.h"

#include "../../Utils/Contracts.h"

namespace Physics_NS {
    /// \brief class for constant Medium
    template < typename T >
    class MediumConstant : public MediumInterface<T> {
    public:
        using Base = MediumInterface<T>;

        /// \param[in] n medium refraction index
        /// \param[in] a medium absorption coefficient
        /// \param[in] u medium scattering coefficient
        /// \param[in] g medium scattering anisotropy coefficient
        /// \param[in] r medium density
        /// \param[in] c medium heat capacity
        /// \param[in] k medium thermal conductivity
        explicit MediumConstant(const T& n = 1,
                                const T& a = 0,
                                const T& u = 0,
                                const T& g = 0,
                                const T& r = 0,
                                const T& c = 0,
                                const T& k = 0) noexcept;
        /// \param[in] properties constant medium MediumProperties
        explicit MediumConstant(const MediumProperties<T>& properties) noexcept;

        T refraction          (const T& t = 0, const T& d = 0) const EXCEPT_INPUT_PARAMS override;
        T absorption          (const T& t = 0, const T& d = 0) const EXCEPT_INPUT_PARAMS override;
        T scattering          (const T& t = 0, const T& d = 0) const EXCEPT_INPUT_PARAMS override;
        T anisotropy          (const T& t = 0, const T& d = 0) const EXCEPT_INPUT_PARAMS override;
        T density             (const T& t = 0, const T& d = 0) const EXCEPT_INPUT_PARAMS override;
        T heat_capacity       (const T& t = 0, const T& d = 0) const EXCEPT_INPUT_PARAMS override;
        T thermal_conductivity(const T& t = 0, const T& d = 0) const EXCEPT_INPUT_PARAMS override;

    protected:
        const T n0; ///< Medium refraction index
        const T a0; ///< Medium absorption coefficient
        const T u0; ///< Medium scattering coefficient
        const T g0; ///< Medium scattering anisotropy coefficient
        const T r0; ///< Medium density
        const T c0; ///< Medium heat capacity
        const T k0; ///< Medium thermal conductivity
    };
}

/******************
 * IMPLEMENTATION *
 ******************/

template < typename T >
Physics_NS::MediumConstant<T>::MediumConstant(const T& n,
                                              const T& a,
                                              const T& u,
                                              const T& g,
                                              const T& r,
                                              const T& c,
                                              const T& k) noexcept
    : Physics_NS::MediumConstant<T>::Base(MediumType::Constant)
    , n0(n)
    , a0(a)
    , u0(u)
    , g0(g)
    , r0(r)
    , c0(c)
    , k0(k) {
}

template < typename T >
Physics_NS::MediumConstant<T>::MediumConstant(const MediumProperties<T>& properties) noexcept
    : Physics_NS::MediumConstant<T>::Base(MediumType::Constant)
    , n0(properties.n0.has_value() ? properties.n0.value() : 1)
    , a0(properties.a0.has_value() ? properties.a0.value() : 0)
    , u0(properties.u0.has_value() ? properties.u0.value() : 0)
    , g0(properties.g0.has_value() ? properties.g0.value() : 0)
    , r0(properties.r0.has_value() ? properties.r0.value() : 0)
    , c0(properties.c0.has_value() ? properties.c0.value() : 0)
    , k0(properties.k0.has_value() ? properties.k0.value() : 0)  {
}

template < typename T >
T Physics_NS::MediumConstant<T>::refraction(const T& t, const T& d) const EXCEPT_INPUT_PARAMS {
    std::ignore = t;
    std::ignore = d;
    return n0;
}

template < typename T >
T Physics_NS::MediumConstant<T>::absorption(const T& t, const T& d) const EXCEPT_INPUT_PARAMS {
    std::ignore = t;
    std::ignore = d;
    return a0;
}

template < typename T >
T Physics_NS::MediumConstant<T>::scattering(const T& t, const T& d) const EXCEPT_INPUT_PARAMS {
    std::ignore = t;
    std::ignore = d;
    return u0;
}

template < typename T >
T Physics_NS::MediumConstant<T>::anisotropy(const T& t, const T& d) const EXCEPT_INPUT_PARAMS {
    std::ignore = t;
    std::ignore = d;
    return g0;
}

template < typename T >
T Physics_NS::MediumConstant<T>::density(const T& t, const T& d) const EXCEPT_INPUT_PARAMS {
    std::ignore = t;
    std::ignore = d;
    return r0;
}

template < typename T >
T Physics_NS::MediumConstant<T>::heat_capacity(const T& t, const T& d) const EXCEPT_INPUT_PARAMS {
    std::ignore = t;
    std::ignore = d;
    return c0;
}

template < typename T >
T Physics_NS::MediumConstant<T>::thermal_conductivity(const T& t, const T& d) const EXCEPT_INPUT_PARAMS {
    std::ignore = t;
    std::ignore = d;
    return k0;
}
