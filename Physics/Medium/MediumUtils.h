#pragma once

#include "MediumArbitrary.h"
#include "MediumConstant.h"
#include "MediumGlass.h"
#include "MediumInterface.h"
#include "MediumLinear.h"
#include "MediumProperties.h"
#include "MediumType.h"

#include <algorithm>
#include <stdexcept>
#include <string>

#ifdef ASSERT_INPUT_PARAMS
    #define EXCEPT_INPUT_PARAMS
#else
    #define EXCEPT_INPUT_PARAMS noexcept
#endif // ASSERT_INPUT_PARAMS

namespace Physics_NS {
    /// Medium type getter from MediumInterface pointer
    /// \param[in] medium MediumInterface pointer
    /// \return MediumType, MediumType::Unknown if cannot convert to the known types
    /// \throw std::invalid_argument If ASSERT_INPUT_PARAMS is defined and cannot convert to the known types
    template < typename T >
    inline MediumType mediumType(MediumInterface<T>* const medium) EXCEPT_INPUT_PARAMS;

    /// Medium type getter from MediumInterface pointer
    /// \param[in] medium Medium name
    /// \return MediumType, MediumType::Unknown if cannot convert to the known types
    /// \throw std::invalid_argument If ASSERT_INPUT_PARAMS is defined and cannot convert to the known types
    inline MediumType mediumType(const std::string& medium) EXCEPT_INPUT_PARAMS;

    /// Validator for MediumProperties
    /// \param[in] properties MediumProperties
    /// \return in case validation succeeded no exception or value is returned
    /// \throw std::logic_error if not all needed properties are available
    template < typename T >
    void validate(const MediumProperties<T>& properties);
    /// Validator for MediumProperties, calls validate
    /// \param[in] properties MediumProperties
    /// \return true if validation succeeded no exception or false otherwise
    template < typename T >
    bool validateSafe(const MediumProperties<T>& properties) noexcept;

    /// Export medium properties to MediumProperties
    /// \param[in] medium MediumInterface pointer
    /// \return MediumProperties for corresponding medium
    template < typename T >
    MediumProperties<T> exportMediumProperties(MediumInterface<T>* const medium) noexcept;
}

/******************
 * IMPLEMENTATION *
 ******************/

template < typename T >
Physics_NS::MediumType Physics_NS::mediumType(Physics_NS::MediumInterface<T>* const medium) EXCEPT_INPUT_PARAMS {
    if (dynamic_cast<Physics_NS::MediumGlass<T>*>(medium))
        return Physics_NS::MediumType::Glass;
    if (dynamic_cast<Physics_NS::MediumConstant<T>*>(medium))
        return Physics_NS::MediumType::Constant;
    if (dynamic_cast<Physics_NS::MediumLinear<T>*>(medium))
        return Physics_NS::MediumType::Linear;
    if (dynamic_cast<Physics_NS::MediumArbitrary<T>*>(medium))
        return Physics_NS::MediumType::Arbitrary;

    #ifdef ASSERT_INPUT_PARAMS
        throw std::invalid_argument("Medium type cannot be evaluated");
    #endif // ASSERT_INPUT_PARAMS

    return Physics_NS::MediumType::Unknown;
}

Physics_NS::MediumType Physics_NS::mediumType(const std::string& medium) EXCEPT_INPUT_PARAMS {
    auto lower = medium;
    std::transform(lower.begin(), lower.end(), lower.begin(), [](unsigned char c) { return std::tolower(c); } );

    if (lower == "glass")
        return Physics_NS::MediumType::Glass;
    if (lower == "constant")
        return Physics_NS::MediumType::Constant;
    if (lower == "linear")
        return Physics_NS::MediumType::Linear;
    if (lower == "arbitrary")
        return Physics_NS::MediumType::Arbitrary;

    #ifdef ASSERT_INPUT_PARAMS
        throw std::invalid_argument("Medium type cannot be evaluated");
    #endif // ASSERT_INPUT_PARAMS

    return Physics_NS::MediumType::Unknown;
}

template < typename T >
void Physics_NS::validate(const Physics_NS::MediumProperties<T>& properties) {
    if (properties.type == Physics_NS::MediumType::Unknown)
        throw std::logic_error("Validation fails for MediumType::Unknown");

    if (properties.type == Physics_NS::MediumType::Glass) {
        if (!properties.n0.has_value())
            throw std::logic_error("Validation fails for MediumType::Glass because there is no n0 property");
        if (properties.n0.value() < 1)
            throw std::logic_error("Validation fails for MediumType::Glass because n0 property is less than 1");

        return;
    }

    if (properties.type == Physics_NS::MediumType::Constant) {
        if (!properties.n0.has_value())
            throw std::logic_error("Validation fails for MediumType::Constant because there is no n0 property");
        if (properties.n0.value() < 1)
            throw std::logic_error("Validation fails for MediumType::Constant because n0 property is less than 1");

        if (!properties.a0.has_value())
            throw std::logic_error("Validation fails for MediumType::Constant because there is no a0 property");
        if (properties.a0.value() < 0)
            throw std::logic_error("Validation fails for MediumType::Constant because a0 property is less than 0");

        if (!properties.u0.has_value())
            throw std::logic_error("Validation fails for MediumType::Constant because there is no u0 property");
        if (properties.u0.value() < 0)
            throw std::logic_error("Validation fails for MediumType::Constant because u0 property is less than 0");

        if (!properties.g0.has_value())
            throw std::logic_error("Validation fails for MediumType::Constant because there is no g0 property");
        if (properties.g0.value() < -1)
            throw std::logic_error("Validation fails for MediumType::Constant because g0 property is less than -1");
        if (properties.g0.value() > +1)
            throw std::logic_error("Validation fails for MediumType::Constant because g0 property is greater than +1");

        return;
    }

    if (properties.type == Physics_NS::MediumType::Linear) {
        if (!properties.n0.has_value())
            throw std::logic_error("Validation fails for MediumType::Linear because there is no n0 property");
        if (properties.n0.value() < 1)
            throw std::logic_error("Validation fails for MediumType::Linear because n0 property is less than 1");
        if (!properties.nT.has_value())
            throw std::logic_error("Validation fails for MediumType::Linear because there is no nT property");
        if (!properties.nD.has_value())
            throw std::logic_error("Validation fails for MediumType::Linear because there is no nD property");

        if (!properties.a0.has_value())
            throw std::logic_error("Validation fails for MediumType::Linear because there is no a0 property");
        if (properties.a0.value() < 0)
            throw std::logic_error("Validation fails for MediumType::Linear because a0 property is less than 0");
        if (!properties.aT.has_value())
            throw std::logic_error("Validation fails for MediumType::Linear because there is no aT property");
        if (!properties.aD.has_value())
            throw std::logic_error("Validation fails for MediumType::Linear because there is no aD property");

        if (!properties.u0.has_value())
            throw std::logic_error("Validation fails for MediumType::Linear because there is no u0 property");
        if (properties.u0.value() < 0)
            throw std::logic_error("Validation fails for MediumType::Linear because u0 property is less than 0");
        if (!properties.uT.has_value())
            throw std::logic_error("Validation fails for MediumType::Linear because there is no uT property");
        if (!properties.uD.has_value())
            throw std::logic_error("Validation fails for MediumType::Linear because there is no uD property");

        if (!properties.g0.has_value())
            throw std::logic_error("Validation fails for MediumType::Linear because there is no g0 property");
        if (properties.g0.value() < -1)
            throw std::logic_error("Validation fails for MediumType::Linear because g0 property is less than -1");
        if (properties.g0.value() > +1)
            throw std::logic_error("Validation fails for MediumType::Linear because g0 property is greater than +1");
        if (!properties.gT.has_value())
            throw std::logic_error("Validation fails for MediumType::Linear because there is no gT property");
        if (!properties.gD.has_value())
            throw std::logic_error("Validation fails for MediumType::Linear because there is no gD property");

        return;
    }

    if (properties.type == Physics_NS::MediumType::Arbitrary) {
        if (!properties.n0.has_value())
            throw std::logic_error("Validation fails for MediumType::Linear because there is no n0 property");
        if (properties.n0.value() < 1)
            throw std::logic_error("Validation fails for MediumType::Linear because n0 property is less than 1");
        if (!properties.nT.has_value())
            throw std::logic_error("Validation fails for MediumType::Linear because there is no nT property");
        if (!properties.nD.has_value())
            throw std::logic_error("Validation fails for MediumType::Linear because there is no nD property");
        if (!properties.nDT.has_value())
            throw std::logic_error("Validation fails for MediumType::Linear because there is no nDT property");

        if (!properties.a0.has_value())
            throw std::logic_error("Validation fails for MediumType::Linear because there is no a0 property");
        if (properties.a0.value() < 0)
            throw std::logic_error("Validation fails for MediumType::Linear because a0 property is less than 0");
        if (!properties.aT.has_value())
            throw std::logic_error("Validation fails for MediumType::Linear because there is no aT property");
        if (!properties.aD.has_value())
            throw std::logic_error("Validation fails for MediumType::Linear because there is no aD property");
        if (!properties.aDT.has_value())
            throw std::logic_error("Validation fails for MediumType::Linear because there is no aDT property");

        if (!properties.u0.has_value())
            throw std::logic_error("Validation fails for MediumType::Linear because there is no u0 property");
        if (properties.u0.value() < 0)
            throw std::logic_error("Validation fails for MediumType::Linear because u0 property is less than 0");
        if (!properties.uT.has_value())
            throw std::logic_error("Validation fails for MediumType::Linear because there is no uT property");
        if (!properties.uD.has_value())
            throw std::logic_error("Validation fails for MediumType::Linear because there is no uD property");
        if (!properties.uDT.has_value())
            throw std::logic_error("Validation fails for MediumType::Linear because there is no uDT property");

        if (!properties.g0.has_value())
            throw std::logic_error("Validation fails for MediumType::Linear because there is no g0 property");
        if (properties.g0.value() < -1)
            throw std::logic_error("Validation fails for MediumType::Linear because g0 property is less than -1");
        if (properties.g0.value() > +1)
            throw std::logic_error("Validation fails for MediumType::Linear because g0 property is greater than +1");
        if (!properties.gT.has_value())
            throw std::logic_error("Validation fails for MediumType::Linear because there is no gT property");
        if (!properties.gD.has_value())
            throw std::logic_error("Validation fails for MediumType::Linear because there is no gD property");
        if (!properties.gDT.has_value())
            throw std::logic_error("Validation fails for MediumType::Linear because there is no gDT property");

        return;
    }
}

template < typename T >
bool Physics_NS::validateSafe(const Physics_NS::MediumProperties<T>& properties) noexcept {
    try {
        validate(properties);
        return true;
    } catch(std::logic_error&) {
        return false;
    }
}

template < typename T >
Physics_NS::MediumProperties<T> Physics_NS::exportMediumProperties(Physics_NS::MediumInterface<T>* const medium) noexcept {
    Physics_NS::MediumProperties<T> result;
    result.type = mediumType(medium);

    if (result.type == Physics_NS::MediumType::Glass) {
        auto casted_medium = dynamic_cast<Physics_NS::MediumGlass<T>*>(medium);
        result.n0 = casted_medium->refraction(0, 0);
    }

    if (result.type == Physics_NS::MediumType::Constant) {
        auto casted_medium = dynamic_cast<Physics_NS::MediumConstant<T>*>(medium);
        result.n0 = casted_medium->refraction(0, 0);
        result.a0 = casted_medium->absorption(0, 0);
        result.u0 = casted_medium->scattering(0, 0);
        result.g0 = casted_medium->anisotropy(0, 0);
    }

    if (result.type == Physics_NS::MediumType::Linear) {
        auto casted_medium = dynamic_cast<Physics_NS::MediumLinear<T>*>(medium);
        result.n0 = casted_medium->refraction(0, 0);
        result.nT = casted_medium->refraction(1, 0) - result.n0.value();
        result.nD = casted_medium->refraction(0, 1) - result.n0.value();

        result.a0 = casted_medium->absorption(0, 0);
        result.aT = casted_medium->absorption(1, 0) - result.a0.value();
        result.aD = casted_medium->absorption(0, 1) - result.a0.value();

        result.u0 = casted_medium->scattering(0, 0);
        result.uT = casted_medium->scattering(1, 0) - result.u0.value();
        result.uD = casted_medium->scattering(0, 1) - result.u0.value();

        result.g0 = casted_medium->anisotropy(0, 0);
        result.gT = casted_medium->anisotropy(1, 0) - result.g0.value();
        result.gD = casted_medium->anisotropy(0, 1) - result.g0.value();
    }

    if (result.type == Physics_NS::MediumType::Arbitrary) {
        auto casted_medium = dynamic_cast<Physics_NS::MediumArbitrary<T>*>(medium);
        result.n0  = casted_medium->refraction(0, 0);
        result.nT  = casted_medium->refraction(1, 0) - result.n0.value();
        result.nD  = casted_medium->refraction(0, 1) - result.n0.value();
        result.nDT = casted_medium->refraction(1, 1) - result.n0.value() - result.nT.value() - result.nD.value();

        result.a0  = casted_medium->absorption(0, 0);
        result.aT  = casted_medium->absorption(1, 0) - result.a0.value();
        result.aD  = casted_medium->absorption(0, 1) - result.a0.value();
        result.aDT = casted_medium->absorption(1, 1) - result.a0.value() - result.aT.value() - result.aD.value();

        result.u0  = casted_medium->scattering(0, 0);
        result.uT  = casted_medium->scattering(1, 0) - result.u0.value();
        result.uD  = casted_medium->scattering(0, 1) - result.u0.value();
        result.uDT = casted_medium->scattering(1, 1) - result.u0.value() - result.uT.value() - result.uD.value();

        result.g0  = casted_medium->anisotropy(0, 0);
        result.gT  = casted_medium->anisotropy(1, 0) - result.g0.value();
        result.gD  = casted_medium->anisotropy(0, 1) - result.g0.value();
        result.gDT = casted_medium->anisotropy(1, 1) - result.g0.value() - result.gT.value() - result.gD.value();
    }

    return result;
}
