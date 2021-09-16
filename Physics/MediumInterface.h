#pragma once

#include <stdexcept>

template < typename T >
class MediumInterface {
public:
    virtual ~MediumInterface() = default;

    /// Medium refraction index
    /// \param[in] temperature tissue temperature
    /// \param[in] degradation tissue degradation from Arrhenius equation
    /// \return Medium refraction index with corresponding temperature and degradation
    virtual T refraction(const T& temperature = 0, const T& degradation = 0) const { throw std::runtime_error("Medium refraction was not implemented"); };

    /// Medium absorption coefficient
    /// \param[in] temperature tissue temperature
    /// \param[in] degradation tissue degradation from Arrhenius equation
    /// \return Medium absorption coefficient with corresponding temperature and degradation
    virtual T absorption(const T& temperature = 0, const T& degradation = 0) const { throw std::runtime_error("Medium absorption was not implemented"); };
    /// Medium scattering coefficient
    /// \param[in] temperature tissue temperature
    /// \param[in] degradation tissue degradation from Arrhenius equation
    /// \return Medium scattering coefficient with corresponding temperature and degradation
    virtual T scattering(const T& temperature = 0, const T& degradation = 0) const { throw std::runtime_error("Medium scattering was not implemented"); };
    /// Medium anisotropy coefficient
    /// \param[in] temperature tissue temperature
    /// \param[in] degradation tissue degradation from Arrhenius equation
    /// \return Medium anisotropy coefficient with corresponding temperature and degradation
    virtual T anisotropy(const T& temperature = 0, const T& degradation = 0) const { throw std::runtime_error("Medium anisotropy was not implemented"); };

    /// Medium interaction coefficient = absorption + scattering
    /// \param[in] temperature tissue temperature
    /// \param[in] degradation tissue degradation from Arrhenius equation
    /// \return Medium interaction coefficient with corresponding temperature and degradation
    virtual T interaction(const T& temperature = 0, const T& degradation = 0) const;
    /// Medium albedo = scattering / interaction
    /// \param[in] temperature tissue temperature
    /// \param[in] degradation tissue degradation from Arrhenius equation
    /// \return Medium albedo with corresponding temperature and degradation
    virtual T albedo(const T& temperature = 0, const T& degradation = 0) const;

    /// Validation if Medium is glass
    /// \return If Medium is glass
    virtual bool isGlass() const { throw std::runtime_error("Medium glass was not implemented"); };
};

/******************
 * IMPLEMENTATION *
 ******************/

template < typename T >
T MediumInterface<T>::interaction(const T& temperature, const T& degradation) const {
    return absorption(temperature, degradation) + scattering(temperature, degradation);
}

template < typename T >
T MediumInterface<T>::albedo(const T& temperature, const T& degradation) const {
    return scattering(temperature, degradation) / interaction(temperature, degradation);
}
