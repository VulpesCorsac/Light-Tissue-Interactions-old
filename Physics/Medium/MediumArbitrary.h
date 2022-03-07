#pragma once

#include "MediumInterface.h"
#include "MediumProperties.h"
#include "MediumType.h"

#include "../../Utils/Contracts.h"

namespace Physics_NS {
    /// \brief class for arbitrary Medium
    template < typename T >
    class MediumArbitrary : public MediumInterface<T> {
    public:
        using Base = MediumInterface<T>;

        /// \param[in] n0  medium refraction index for T = 0, D = 0
        /// \param[in] nT  d(Medium refraction index)/d(Temperature)
        /// \param[in] nD  d(Medium refraction index)/d(Degradation)
        /// \param[in] nDT d2(Medium refraction index)/d(Degradation)d(Temperature)
        /// \param[in] a0  medium absorption coefficient for T = 0, D = 0
        /// \param[in] aT  d(Medium absorption coefficient)/d(Temperature)
        /// \param[in] aD  d(Medium absorption coefficient)/d(Degradation)
        /// \param[in] aDT d2(Medium absorption coefficient)/d(Degradation)d(Temperature)
        /// \param[in] u0  medium scattering coefficient for T = 0, D = 0
        /// \param[in] uT  d(Medium scattering coefficient)/d(Temperature)
        /// \param[in] uD  d(Medium scattering coefficient)/d(Degradation)
        /// \param[in] uDT d2(Medium scattering coefficient)/d(Degradation)d(Temperature)
        /// \param[in] g0  medium scattering anisotropy coefficient for T = 0, D = 0
        /// \param[in] gT  d(Medium scattering anisotropy coefficient)/d(Temperature)
        /// \param[in] gD  d(Medium scattering anisotropy coefficient)/d(Degradation)
        /// \param[in] gDT d2(Medium scattering anisotropy coefficient)/d(Degradation)d(Temperature)
        /// \param[in] r0  Medium density
        /// \param[in] rT  d(Medium density coefficient)/d(Temperature)
        /// \param[in] rD  d(Medium density coefficient)/d(Degradation)
        /// \param[in] rDT d2(Medium density)/d(Degradation)d(Temperature)
        /// \param[in] c0  Medium heat capacity
        /// \param[in] cT  d(Medium heat capacity coefficient)/d(Temperature)
        /// \param[in] cD  d(Medium heat capacity coefficient)/d(Degradation)
        /// \param[in] cDT d2(Medium heat capacity)/d(Degradation)d(Temperature)
        /// \param[in] k0  Medium thermal conductivity
        /// \param[in] kT  d(Medium thermal conductivity coefficient)/d(Temperature)
        /// \param[in] kD  d(Medium thermal conductivity coefficient)/d(Degradation)
        /// \param[in] kDT d2(Medium thermal conductivity)/d(Degradation)d(Temperature)
        explicit MediumArbitrary(const T& n0 = 1, const T& nT = 0, const T& nD = 0, const T& nDT = 0,
                                 const T& a0 = 0, const T& aT = 0, const T& aD = 0, const T& aDT = 0,
                                 const T& u0 = 0, const T& uT = 0, const T& uD = 0, const T& uDT = 0,
                                 const T& g0 = 0, const T& gT = 0, const T& gD = 0, const T& gDT = 0,
                                 const T& r0 = 0, const T& rT = 0, const T& rD = 0, const T& rDT = 0,
                                 const T& c0 = 0, const T& cT = 0, const T& cD = 0, const T& cDT = 0,
                                 const T& k0 = 0, const T& kT = 0, const T& kD = 0, const T& kDT = 0) noexcept;
        /// \param[in] properties arbitrary medium MediumProperties
        explicit MediumArbitrary(const MediumProperties<T>& properties) noexcept;

        T refraction          (const T& t = 0, const T& d = 0) const EXCEPT_INPUT_PARAMS override;
        T absorption          (const T& t = 0, const T& d = 0) const EXCEPT_INPUT_PARAMS override;
        T scattering          (const T& t = 0, const T& d = 0) const EXCEPT_INPUT_PARAMS override;
        T anisotropy          (const T& t = 0, const T& d = 0) const EXCEPT_INPUT_PARAMS override;
        T density             (const T& t = 0, const T& d = 0) const EXCEPT_INPUT_PARAMS override;
        T heat_capacity       (const T& t = 0, const T& d = 0) const EXCEPT_INPUT_PARAMS override;
        T thermal_conductivity(const T& t = 0, const T& d = 0) const EXCEPT_INPUT_PARAMS override;

    protected:
        const T n0 ; ///< Medium refraction index for T = 0, D = 0
        const T nT ; ///< d(Medium refraction index)/d(Temperature)
        const T nD ; ///< d(Medium refraction index)/d(Degradation)
        const T nDT; ///< d2(Medium refraction index)/d(Degradation)d(Temperature)

        const T a0 ; ///< Medium absorption coefficient for T = 0, D = 0
        const T aT ; ///< d(Medium absorption coefficient)/d(Temperature)
        const T aD ; ///< d(Medium absorption coefficient)/d(Degradation)
        const T aDT; ///< d2(Medium absorption coefficient)/d(Degradation)d(Temperature)

        const T u0 ; ///< Medium scattering coefficient for T = 0, D = 0
        const T uT ; ///< d(Medium scattering coefficient)/d(Temperature)
        const T uD ; ///< d(Medium scattering coefficient)/d(Degradation)
        const T uDT; ///< d2(Medium scattering coefficient)/d(Degradation)d(Temperature)

        const T g0 ; ///< Medium scattering anisotropy coefficient for T = 0, D = 0
        const T gT ; ///< d(Medium scattering anisotropy coefficient)/d(Temperature)
        const T gD ; ///< d(Medium scattering anisotropy coefficient)/d(Degradation)
        const T gDT; ///< d2(Medium scattering anisotropy coefficient)/d(Degradation)d(Temperature)

        const T r0 ; ///< Medium density
        const T rT ; ///< d(Medium density coefficient)/d(Temperature)
        const T rD ; ///< d(Medium density coefficient)/d(Degradation)
        const T rDT; ///< d2(Medium density)/d(Degradation)d(Temperature)

        const T c0 ; ///< Medium heat capacity
        const T cT ; ///< d(Medium heat capacity coefficient)/d(Temperature)
        const T cD ; ///< d(Medium heat capacity coefficient)/d(Degradation)
        const T cDT; ///< d2(Medium heat capacity)/d(Degradation)d(Temperature)

        const T k0 ; ///< Medium thermal conductivity
        const T kT ; ///< d(Medium thermal conductivity coefficient)/d(Temperature)
        const T kD ; ///< d(Medium thermal conductivity coefficient)/d(Degradation)
        const T kDT; ///< d2(Medium thermal conductivity)/d(Degradation)d(Temperature)
    };
}

/******************
 * IMPLEMENTATION *
 ******************/

template < typename T >
Physics_NS::MediumArbitrary<T>::MediumArbitrary(const T& n0, const T& nT, const T& nD, const T& nDT,
                                                const T& a0, const T& aT, const T& aD, const T& aDT,
                                                const T& u0, const T& uT, const T& uD, const T& uDT,
                                                const T& g0, const T& gT, const T& gD, const T& gDT,
                                                const T& r0, const T& rT, const T& rD, const T& rDT,
                                                const T& c0, const T& cT, const T& cD, const T& cDT,
                                                const T& k0, const T& kT, const T& kD, const T& kDT) noexcept
    : Physics_NS::MediumArbitrary<T>::Base(MediumType::Arbitrary)
    , n0(n0), nT(nT), nD(nD), nDT(nDT)
    , a0(a0), aT(aT), aD(aD), aDT(aDT)
    , u0(u0), uT(uT), uD(uD), uDT(uDT)
    , g0(g0), gT(gT), gD(gD), gDT(gDT)
    , r0(r0), rT(rT), rD(rD), rDT(rDT)
    , c0(c0), cT(cT), cD(cD), cDT(cDT)
    , k0(k0), kT(kT), kD(kD), kDT(kDT) {
}

template < typename T >
Physics_NS::MediumArbitrary<T>::MediumArbitrary(const MediumProperties<T>& properties) noexcept
    : Physics_NS::MediumArbitrary<T>::Base(MediumType::Arbitrary)
    , n0 (properties.n0.has_value()  ? properties.n0.value()  : 1)
    , nT (properties.nT.has_value()  ? properties.nT.value()  : 0)
    , nD (properties.nD.has_value()  ? properties.nD.value()  : 0)
    , nDT(properties.nDT.has_value() ? properties.nDT.value() : 0)
    , a0 (properties.a0.has_value()  ? properties.a0.value()  : 0)
    , aT (properties.aT.has_value()  ? properties.aT.value()  : 0)
    , aD (properties.aD.has_value()  ? properties.aD.value()  : 0)
    , aDT(properties.aDT.has_value() ? properties.aDT.value() : 0)
    , u0 (properties.u0.has_value()  ? properties.u0.value()  : 0)
    , uT (properties.uT.has_value()  ? properties.uT.value()  : 0)
    , uD (properties.uD.has_value()  ? properties.uD.value()  : 0)
    , uDT(properties.uDT.has_value() ? properties.uDT.value() : 0)
    , g0 (properties.g0.has_value()  ? properties.g0.value()  : 0)
    , gT (properties.gT.has_value()  ? properties.gT.value()  : 0)
    , gD (properties.gD.has_value()  ? properties.gD.value()  : 0)
    , gDT(properties.gDT.has_value() ? properties.gDT.value() : 0)
    , r0 (properties.r0.has_value()  ? properties.r0.value()  : 0)
    , rT (properties.rT.has_value()  ? properties.rT.value()  : 0)
    , rD (properties.rD.has_value()  ? properties.rD.value()  : 0)
    , rDT(properties.rDT.has_value() ? properties.rDT.value() : 0)
    , c0 (properties.c0.has_value()  ? properties.c0.value()  : 0)
    , cT (properties.cT.has_value()  ? properties.cT.value()  : 0)
    , cD (properties.cD.has_value()  ? properties.cD.value()  : 0)
    , cDT(properties.cDT.has_value() ? properties.cDT.value() : 0)
    , k0 (properties.k0.has_value()  ? properties.k0.value()  : 0)
    , kT (properties.kT.has_value()  ? properties.kT.value()  : 0)
    , kD (properties.kD.has_value()  ? properties.kD.value()  : 0)
    , kDT(properties.kDT.has_value() ? properties.kDT.value() : 0) {
}

template < typename T >
T Physics_NS::MediumArbitrary<T>::refraction(const T& t, const T& d) const EXCEPT_INPUT_PARAMS {
    return n0 + nT*t + nD*d + nDT*t*d;
}

template < typename T >
T Physics_NS::MediumArbitrary<T>::absorption(const T& t, const T& d) const EXCEPT_INPUT_PARAMS {
    return a0 + aT*t + aD*d + aDT*t*d;
}

template < typename T >
T Physics_NS::MediumArbitrary<T>::scattering(const T& t, const T& d) const EXCEPT_INPUT_PARAMS {
    return u0 + uT*t + uD*d + uDT*t*d;
}

template < typename T >
T Physics_NS::MediumArbitrary<T>::anisotropy(const T& t, const T& d) const EXCEPT_INPUT_PARAMS {
    return g0 + gT*t + gD*d + gDT*t*d;
}

template < typename T >
T Physics_NS::MediumArbitrary<T>::density(const T& t, const T& d) const EXCEPT_INPUT_PARAMS {
    return r0 + rT*t + rD*d + rDT*t*d;
}

template < typename T >
T Physics_NS::MediumArbitrary<T>::heat_capacity(const T& t, const T& d) const EXCEPT_INPUT_PARAMS {
    return c0 + cT*t + cD*d + cDT*t*d;
}

template < typename T >
T Physics_NS::MediumArbitrary<T>::thermal_conductivity(const T& t, const T& d) const EXCEPT_INPUT_PARAMS {
    return k0 + kT*t + kD*d + kDT*t*d;
}
