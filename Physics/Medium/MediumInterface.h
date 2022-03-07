#pragma once

#include "MediumType.h"

#include "../../Utils/Contracts.h"

#include <tuple>

namespace Physics_NS {
    /// \brief Interface class for Medium
    template < typename T >
    class MediumInterface {
    public:
        /// \param[in] type is used to set in derived classes
        MediumInterface(const MediumType& type = MediumType::Unknown) noexcept;

        /// virtual destructor for interface
        virtual ~MediumInterface() = default;

        /// Medium refraction index
        /// \param[in] t tissue temperature
        /// \param[in] d tissue degradation from Arrhenius equation
        /// \return Medium refraction index with corresponding temperature and degradation
        /// \throw std::runtime_error Interface throws exception
        virtual T refraction(const T& t = 0, const T& d = 0) const EXCEPT_INPUT_PARAMS;

        /// Medium absorption coefficient
        /// \param[in] t tissue temperature
        /// \param[in] degradation tissue degradation from Arrhenius equation
        /// \return Medium absorption coefficient with corresponding temperature and degradation
        /// \throw std::runtime_error Interface throws exception
        virtual T absorption(const T& t = 0, const T& d = 0) const EXCEPT_INPUT_PARAMS;
        /// Medium scattering coefficient
        /// \param[in] t tissue temperature
        /// \param[in] d tissue degradation from Arrhenius equation
        /// \return Medium scattering coefficient with corresponding temperature and degradation
        /// \throw std::runtime_error Interface throws exception
        virtual T scattering(const T& t = 0, const T& d = 0) const EXCEPT_INPUT_PARAMS;
        /// Medium anisotropy coefficient
        /// \param[in] t tissue temperature
        /// \param[in] d tissue degradation from Arrhenius equation
        /// \return Medium anisotropy coefficient with corresponding temperature and degradation
        /// \throw std::runtime_error Interface throws exception
        virtual T anisotropy(const T& t = 0, const T& d = 0) const EXCEPT_INPUT_PARAMS;

        /// Medium interaction coefficient = absorption + scattering
        /// \param[in] t tissue temperature
        /// \param[in] d tissue degradation from Arrhenius equation
        /// \return Medium interaction coefficient with corresponding temperature and degradation
        virtual T interaction(const T& t = 0, const T& d = 0) const EXCEPT_INPUT_PARAMS;
        /// Medium albedo = scattering / interaction
        /// \param[in] t tissue temperature
        /// \param[in] d tissue degradation from Arrhenius equation
        /// \return Medium albedo with corresponding temperature and degradation
        virtual T albedo(const T& t = 0, const T& d = 0) const EXCEPT_INPUT_PARAMS;

        /// Medium density
        /// \param[in] t tissue temperature
        /// \param[in] d tissue degradation from Arrhenius equation
        /// \return Medium density with corresponding temperature and degradation
        /// \throw std::runtime_error Interface throws exception
        virtual T density(const T& t = 0, const T& d = 0) const EXCEPT_INPUT_PARAMS;
        /// Medium heat capacity
        /// \param[in] t tissue temperature
        /// \param[in] d tissue degradation from Arrhenius equation
        /// \return Medium heat capacity with corresponding temperature and degradation
        /// \throw std::runtime_error Interface throws exception
        virtual T heat_capacity(const T& t = 0, const T& d = 0) const EXCEPT_INPUT_PARAMS;
        /// Medium thermal conductivity
        /// \param[in] t tissue temperature
        /// \param[in] d tissue degradation from Arrhenius equation
        /// \return Medium thermal conductivity with corresponding temperature and degradation
        /// \throw std::runtime_error Interface throws exception
        virtual T thermal_conductivity(const T& t = 0, const T& d = 0) const EXCEPT_INPUT_PARAMS;
        /// Medium thermal diffusivity = thermal conductivity / (density * heat capacity)
        /// \param[in] t tissue temperature
        /// \param[in] d tissue degradation from Arrhenius equation
        /// \return Medium thermal diffusivity with corresponding temperature and degradation
        virtual T thermal_diffusivity(const T& t = 0, const T& d = 0) const EXCEPT_INPUT_PARAMS;

    public:
        MediumType type; ///< medium type
    };
}

/******************
 * IMPLEMENTATION *
 ******************/

template < typename T >
Physics_NS::MediumInterface<T>::MediumInterface(const MediumType& type) noexcept
    : type(type) {
}

template < typename T >
T Physics_NS::MediumInterface<T>::interaction(const T& t, const T& d) const EXCEPT_INPUT_PARAMS {
    return absorption(t, d) + scattering(t, d);
}

template < typename T >
T Physics_NS::MediumInterface<T>::albedo(const T& t, const T& d) const EXCEPT_INPUT_PARAMS {
    return scattering(t, d) / interaction(t, d);
}

template < typename T >
T Physics_NS::MediumInterface<T>::refraction(const T& t, const T& d) const EXCEPT_INPUT_PARAMS {
    std::ignore = t;
    std::ignore = d;
    FAIL_RUNTIME_CONTRACT("Medium refraction was not implemented");
    return 0;
}

template < typename T >
T Physics_NS::MediumInterface<T>::absorption(const T& t, const T& d) const EXCEPT_INPUT_PARAMS {
    std::ignore = t;
    std::ignore = d;
    FAIL_RUNTIME_CONTRACT("Medium absorption was not implemented");
    return 0;
}

template < typename T >
T Physics_NS::MediumInterface<T>::scattering(const T& t, const T& d) const EXCEPT_INPUT_PARAMS {
    std::ignore = t;
    std::ignore = d;
    FAIL_RUNTIME_CONTRACT("Medium scattering was not implemented");
    return 0;
}

template < typename T >
T Physics_NS::MediumInterface<T>::anisotropy(const T& t, const T& d) const EXCEPT_INPUT_PARAMS {
    std::ignore = t;
    std::ignore = d;
    FAIL_RUNTIME_CONTRACT("Medium anisotropy was not implemented");
    return 0;
}

template < typename T >
T Physics_NS::MediumInterface<T>::density(const T& t, const T& d) const EXCEPT_INPUT_PARAMS {
    std::ignore = t;
    std::ignore = d;
    FAIL_RUNTIME_CONTRACT("Medium density was not implemented");
    return 0;
}

template < typename T >
T Physics_NS::MediumInterface<T>::heat_capacity(const T& t, const T& d) const EXCEPT_INPUT_PARAMS {
    std::ignore = t;
    std::ignore = d;
    FAIL_RUNTIME_CONTRACT("Medium heat capacity was not implemented");
    return 0;
}

template < typename T >
T Physics_NS::MediumInterface<T>::thermal_conductivity(const T& t, const T& d) const EXCEPT_INPUT_PARAMS {
    std::ignore = t;
    std::ignore = d;
    FAIL_RUNTIME_CONTRACT("Medium thermal conductivity was not implemented");
    return 0;
}

template < typename T >
T Physics_NS::MediumInterface<T>::thermal_diffusivity(const T& t, const T& d) const EXCEPT_INPUT_PARAMS {
    return thermal_conductivity(t, d) / density(t, d) / heat_capacity(t, d);
}
