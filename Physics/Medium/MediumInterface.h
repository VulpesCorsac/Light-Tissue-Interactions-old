#pragma once

#include "MediumType.h"

#include <stdexcept>

namespace Physics_NS {
    /// \brief Interface class for Medium
    template < typename T >
    class MediumInterface {
    public:
        virtual ~MediumInterface() = default;

        /// Medium refraction index
        /// \param[in] t tissue temperature
        /// \param[in] d tissue degradation from Arrhenius equation
        /// \return Medium refraction index with corresponding temperature and degradation
        /// \throw std::runtime_error Interface throws exception
        virtual T refraction(const T& t = 0, const T& d = 0) const;

        /// Medium absorption coefficient
        /// \param[in] t tissue temperature
        /// \param[in] degradation tissue degradation from Arrhenius equation
        /// \return Medium absorption coefficient with corresponding temperature and degradation
        /// \throw std::runtime_error Interface throws exception
        virtual T absorption(const T& t = 0, const T& d = 0) const;
        /// Medium scattering coefficient
        /// \param[in] t tissue temperature
        /// \param[in] d tissue degradation from Arrhenius equation
        /// \return Medium scattering coefficient with corresponding temperature and degradation
        /// \throw std::runtime_error Interface throws exception
        virtual T scattering(const T& t = 0, const T& d = 0) const;
        /// Medium anisotropy coefficient
        /// \param[in] t tissue temperature
        /// \param[in] d tissue degradation from Arrhenius equation
        /// \return Medium anisotropy coefficient with corresponding temperature and degradation
        /// \throw std::runtime_error Interface throws exception
        virtual T anisotropy(const T& t = 0, const T& d = 0) const;

        /// Medium interaction coefficient = absorption + scattering
        /// \param[in] t tissue temperature
        /// \param[in] d tissue degradation from Arrhenius equation
        /// \return Medium interaction coefficient with corresponding temperature and degradation
        virtual T interaction(const T& t = 0, const T& d = 0) const;
        /// Medium albedo = scattering / interaction
        /// \param[in] t tissue temperature
        /// \param[in] d tissue degradation from Arrhenius equation
        /// \return Medium albedo with corresponding temperature and degradation
        virtual T albedo(const T& t = 0, const T& d = 0) const;

    public:
        MediumType type = MediumType::Unknown; ///< tissue type
    };
}

/******************
 * IMPLEMENTATION *
 ******************/

template < typename T >
T Physics_NS::MediumInterface<T>::interaction(const T& t, const T& d) const {
    return absorption(t, d) + scattering(t, d);
}

template < typename T >
T Physics_NS::MediumInterface<T>::albedo(const T& t, const T& d) const {
    return scattering(t, d) / interaction(t, d);
}

template < typename T >
T Physics_NS::MediumInterface<T>::refraction(const T& t, const T& d) const {
    throw std::runtime_error("Medium refraction was not implemented");
}

template < typename T >
T Physics_NS::MediumInterface<T>::absorption(const T& t, const T& d) const {
    throw std::runtime_error("Medium absorption was not implemented");
}

template < typename T >
T Physics_NS::MediumInterface<T>::scattering(const T& t, const T& d) const {
    throw std::runtime_error("Medium scattering was not implemented");
}

template < typename T >
T Physics_NS::MediumInterface<T>::anisotropy(const T& t, const T& d) const {
    throw std::runtime_error("Medium anisotropy was not implemented");
}
