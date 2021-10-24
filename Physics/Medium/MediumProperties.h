#pragma once

#include "MediumType.h"

#include <optional>

namespace Physics_NS {
    /// \brief Full properties for Medium
    template < typename T >
    struct MediumProperties {
        /// \param[in] type MediumType type
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
        MediumProperties(MediumType type = MediumType::Unknown,
                         std::optional<T> n0  = std::nullopt,
                         std::optional<T> nT  = std::nullopt,
                         std::optional<T> nD  = std::nullopt,
                         std::optional<T> nDT = std::nullopt,
                         std::optional<T> a0  = std::nullopt,
                         std::optional<T> aT  = std::nullopt,
                         std::optional<T> aD  = std::nullopt,
                         std::optional<T> aDT = std::nullopt,
                         std::optional<T> u0  = std::nullopt,
                         std::optional<T> uT  = std::nullopt,
                         std::optional<T> uD  = std::nullopt,
                         std::optional<T> uDT = std::nullopt,
                         std::optional<T> g0  = std::nullopt,
                         std::optional<T> gT  = std::nullopt,
                         std::optional<T> gD  = std::nullopt,
                         std::optional<T> gDT = std::nullopt,
                         std::optional<T> r0  = std::nullopt,
                         std::optional<T> rT  = std::nullopt,
                         std::optional<T> rD  = std::nullopt,
                         std::optional<T> rDT = std::nullopt,
                         std::optional<T> c0  = std::nullopt,
                         std::optional<T> cT  = std::nullopt,
                         std::optional<T> cD  = std::nullopt,
                         std::optional<T> cDT = std::nullopt,
                         std::optional<T> k0  = std::nullopt,
                         std::optional<T> kT  = std::nullopt,
                         std::optional<T> kD  = std::nullopt,
                         std::optional<T> kDT = std::nullopt) noexcept;

        /// Medium type
        MediumType type;

        /// Medium refraction index at Temperature = 0, Degradation = 0
        std::optional<T> n0;
        /// d(Medium refraction index)/d(Temperature)
        std::optional<T> nT;
        /// d(Medium refraction index)/d(Degradation)
        std::optional<T> nD;
        /// d2(Medium refraction index)/d(Degradation)d(Temperature). Will be temporarily used for arbitrary medium
        std::optional<T> nDT;

        /// Medium absorption coefficient at Temperature = 0, Degradation = 0
        std::optional<T> a0;
        /// d(Medium absorption coefficient)/d(Temperature)
        std::optional<T> aT;
        /// d(Medium absorption coefficient)/d(Degradation)
        std::optional<T> aD;
        /// d2(Medium absorption coefficient)/d(Degradation)d(Temperature). Will be temporarily used for arbitrary medium
        std::optional<T> aDT;

        /// Medium scattering coefficient at Temperature = 0, Degradation = 0
        std::optional<T> u0;
        /// d(Medium scattering coefficient)/d(Temperature)
        std::optional<T> uT;
        /// d(Medium scattering coefficient)/d(Degradation)
        std::optional<T> uD;
        /// d2(Medium scattering coefficient)/d(Degradation)d(Temperature). Will be temporarily used for arbitrary medium
        std::optional<T> uDT;

        /// Medium scattering anisotropy coefficient at Temperature = 0, Degradation = 0
        std::optional<T> g0;
        /// d(Medium scattering anisotropy coefficient)/d(Temperature)
        std::optional<T> gT;
        /// d(Medium scattering anisotropy coefficient)/d(Degradation)
        std::optional<T> gD;
        /// d2(Medium scattering anisotropy coefficient)/d(Degradation)d(Temperature). Will be temporarily used for arbitrary medium
        std::optional<T> gDT;

        /// Medium density at Temperature = 0, Degradation = 0
        std::optional<T> r0;
        /// d(Medium density)/d(Temperature)
        std::optional<T> rT;
        /// d(Medium density)/d(Degradation)
        std::optional<T> rD;
        /// d2(Medium density)/d(Degradation)d(Temperature). Will be temporarily used for arbitrary medium
        std::optional<T> rDT;

        /// Medium heat capacity at Temperature = 0, Degradation = 0
        std::optional<T> c0;
        /// d(Medium heat capacity)/d(Temperature)
        std::optional<T> cT;
        /// d(Medium heat capacity)/d(Degradation)
        std::optional<T> cD;
        /// d2(Medium heat capacity)/d(Degradation)d(Temperature). Will be temporarily used for arbitrary medium
        std::optional<T> cDT;

        /// Medium thermal conductivity at Temperature = 0, Degradation = 0
        std::optional<T> k0;
        /// d(Medium thermal conductivity)/d(Temperature)
        std::optional<T> kT;
        /// d(Medium thermal conductivity)/d(Degradation)
        std::optional<T> kD;
        /// d2(Medium thermal conductivity)/d(Degradation)d(Temperature). Will be temporarily used for arbitrary medium
        std::optional<T> kDT;
    };
}

/******************
 * IMPLEMENTATION *
 ******************/

template < typename T >
Physics_NS::MediumProperties<T>::MediumProperties(Physics_NS::MediumType type,
                                                  std::optional<T> n0, std::optional<T> nT, std::optional<T> nD, std::optional<T> nDT,
                                                  std::optional<T> a0, std::optional<T> aT, std::optional<T> aD, std::optional<T> aDT,
                                                  std::optional<T> u0, std::optional<T> uT, std::optional<T> uD, std::optional<T> uDT,
                                                  std::optional<T> g0, std::optional<T> gT, std::optional<T> gD, std::optional<T> gDT,
                                                  std::optional<T> r0, std::optional<T> rT, std::optional<T> rD, std::optional<T> rDT,
                                                  std::optional<T> c0, std::optional<T> cT, std::optional<T> cD, std::optional<T> cDT,
                                                  std::optional<T> k0, std::optional<T> kT, std::optional<T> kD, std::optional<T> kDT) noexcept
    : type(type)
    , n0(n0), nT(nT), nD(nD), nDT(nDT)
    , a0(a0), aT(aT), aD(aD), aDT(aDT)
    , u0(u0), uT(uT), uD(uD), uDT(uDT)
    , g0(g0), gT(gT), gD(gD), gDT(gDT)
    , r0(r0), rT(rT), rD(rD), rDT(rDT)
    , c0(c0), cT(cT), cD(cD), cDT(cDT)
    , k0(k0), kT(kT), kD(kD), kDT(kDT) {
}
