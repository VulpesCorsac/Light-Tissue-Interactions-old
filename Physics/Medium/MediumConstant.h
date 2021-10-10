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
        /// \param[in] a medium absorption coefficient
        /// \param[in] u medium scattering coefficient
        /// \param[in] g medium scattering anisotropy coefficient
        explicit MediumConstant(const T& n = 1,
                                const T& a = 0,
                                const T& u = 0,
                                const T& g = 0) noexcept;
        /// \param[in] properties constant medium MediumProperties
        explicit MediumConstant(const MediumProperties<T>& properties) noexcept;

        T refraction(const T& t = 0, const T& d = 0) const override;
        T absorption(const T& t = 0, const T& d = 0) const override;
        T scattering(const T& t = 0, const T& d = 0) const override;
        T anisotropy(const T& t = 0, const T& d = 0) const override;

    protected:
        const T n0; ///< Medium refraction index

        const T a0; ///< Medium absorption coefficient
        const T u0; ///< Medium scattering coefficient
        const T g0; ///< Medium scattering anisotropy coefficient
    };
}

/******************
 * IMPLEMENTATION *
 ******************/

template < typename T >
Physics_NS::MediumConstant<T>::MediumConstant(const T& n,
                                              const T& a,
                                              const T& u,
                                              const T& g) noexcept
    : MediumInterface<T>(MediumType::Constant)
    , n0(n)
    , a0(a)
    , u0(u)
    , g0(g) {
}

template < typename T >
Physics_NS::MediumConstant<T>::MediumConstant(const MediumProperties<T>& properties) noexcept
    : MediumInterface<T>(MediumType::Constant)
    , n0(properties.n0.has_value() ? properties.n0.value() : 1)
    , a0(properties.a0.has_value() ? properties.a0.value() : 0)
    , u0(properties.u0.has_value() ? properties.u0.value() : 0)
    , g0(properties.g0.has_value() ? properties.g0.value() : 0) {
}

template < typename T >
T Physics_NS::MediumConstant<T>::refraction(const T& t, const T& d) const {
    std::ignore = t;
    std::ignore = d;
    return n0;
}

template < typename T >
T Physics_NS::MediumConstant<T>::absorption(const T& t, const T& d) const {
    std::ignore = t;
    std::ignore = d;
    return a0;
}

template < typename T >
T Physics_NS::MediumConstant<T>::scattering(const T& t, const T& d) const {
    std::ignore = t;
    std::ignore = d;
    return u0;
}

template < typename T >
T Physics_NS::MediumConstant<T>::anisotropy(const T& t, const T& d) const {
    std::ignore = t;
    std::ignore = d;
    return g0;
}
