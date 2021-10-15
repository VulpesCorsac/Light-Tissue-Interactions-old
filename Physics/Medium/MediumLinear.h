#pragma once

#include "MediumInterface.h"
#include "MediumProperties.h"
#include "MediumType.h"

namespace Physics_NS {
    /// \brief class for linear Medium
    template < typename T >
    class MediumLinear : public MediumInterface<T> {
    public:
        using Base = MediumInterface<T>;

        /// \param[in] n0 medium refraction index for T = 0, D = 0
        /// \param[in] nT d(Medium refraction index)/d(Temperature)
        /// \param[in] nD d(Medium refraction index)/d(Degradation)
        /// \param[in] a0 medium absorption coefficient for T = 0, D = 0
        /// \param[in] aT d(Medium absorption coefficient)/d(Temperature)
        /// \param[in] aD d(Medium absorption coefficient)/d(Degradation)
        /// \param[in] u0 medium scattering coefficient for T = 0, D = 0
        /// \param[in] uT d(Medium scattering coefficient)/d(Temperature)
        /// \param[in] uD d(Medium scattering coefficient)/d(Degradation)
        /// \param[in] g0 medium scattering anisotropy coefficient for T = 0, D = 0
        /// \param[in] gT d(Medium scattering anisotropy coefficient)/d(Temperature)
        /// \param[in] gD d(Medium scattering anisotropy coefficient)/d(Degradation)
        explicit MediumLinear(const T& n0 = 1, const T& nT = 0, const T& nD = 0,
                              const T& a0 = 0, const T& aT = 0, const T& aD = 0,
                              const T& u0 = 0, const T& uT = 0, const T& uD = 0,
                              const T& g0 = 0, const T& gT = 0, const T& gD = 0) noexcept;
        /// \param[in] properties linear medium MediumProperties
        explicit MediumLinear(const MediumProperties<T>& properties) noexcept;

        T refraction(const T& t = 0, const T& d = 0) const override;
        T absorption(const T& t = 0, const T& d = 0) const override;
        T scattering(const T& t = 0, const T& d = 0) const override;
        T anisotropy(const T& t = 0, const T& d = 0) const override;

    protected:
        const T n0; ///< Medium refraction index for T = 0, D = 0
        const T nT; ///< d(Medium refraction index)/d(Temperature)
        const T nD; ///< d(Medium refraction index)/d(Degradation)

        const T a0; ///< Medium absorption coefficient for T = 0, D = 0
        const T aT; ///< d(Medium absorption coefficient)/d(Temperature)
        const T aD; ///< d(Medium absorption coefficient)/d(Degradation)

        const T u0; ///< Medium scattering coefficient for T = 0, D = 0
        const T uT; ///< d(Medium scattering coefficient)/d(Temperature)
        const T uD; ///< d(Medium scattering coefficient)/d(Degradation)

        const T g0; ///< Medium scattering anisotropy coefficient for T = 0, D = 0
        const T gT; ///< d(Medium scattering anisotropy coefficient)/d(Temperature)
        const T gD; ///< d(Medium scattering anisotropy coefficient)/d(Degradation)
    };
}

/******************
 * IMPLEMENTATION *
 ******************/

template < typename T >
Physics_NS::MediumLinear<T>::MediumLinear(const T& n0, const T& nT, const T& nD,
                                          const T& a0, const T& aT, const T& aD,
                                          const T& u0, const T& uT, const T& uD,
                                          const T& g0, const T& gT, const T& gD) noexcept
    : Physics_NS::MediumLinear<T>::Base(MediumType::Linear)
    , n0(n0), nT(nT), nD(nD)
    , a0(a0), aT(aT), aD(aD)
    , u0(u0), uT(uT), uD(uD)
    , g0(g0), gT(gT), gD(gD) {
}

template < typename T >
Physics_NS::MediumLinear<T>::MediumLinear(const MediumProperties<T>& properties) noexcept
    : Physics_NS::MediumLinear<T>::Base(MediumType::Linear)
    , n0(properties.n0.has_value() ? properties.n0.value() : 1)
    , nT(properties.nT.has_value() ? properties.nT.value() : 0)
    , nD(properties.nD.has_value() ? properties.nD.value() : 0)
    , a0(properties.a0.has_value() ? properties.a0.value() : 0)
    , aT(properties.aT.has_value() ? properties.aT.value() : 0)
    , aD(properties.aD.has_value() ? properties.aD.value() : 0)
    , u0(properties.u0.has_value() ? properties.u0.value() : 0)
    , uT(properties.uT.has_value() ? properties.uT.value() : 0)
    , uD(properties.uD.has_value() ? properties.uD.value() : 0)
    , g0(properties.g0.has_value() ? properties.g0.value() : 0)
    , gT(properties.gT.has_value() ? properties.gT.value() : 0)
    , gD(properties.gD.has_value() ? properties.gD.value() : 0) {
}

template < typename T >
T Physics_NS::MediumLinear<T>::refraction(const T& t, const T& d) const {
    return n0 + nT*t + nD*d;
}

template < typename T >
T Physics_NS::MediumLinear<T>::absorption(const T& t, const T& d) const {
    return a0 + aT*t + aD*d;
}

template < typename T >
T Physics_NS::MediumLinear<T>::scattering(const T& t, const T& d) const {
    return u0 + uT*t + uD*d;
}

template < typename T >
T Physics_NS::MediumLinear<T>::anisotropy(const T& t, const T& d) const {
    return g0 + gT*t + gD*d;
}
