#pragma once

#include "MediumArbitrary.h"
#include "MediumConstant.h"
#include "MediumGlass.h"
#include "MediumInterface.h"
#include "MediumLinear.h"
#include "MediumProperties.h"
#include "MediumType.h"
#include "MediumTypeStrings.h"

#include "../../Utils/Contracts.h"
#include "../../Utils/StringUtils.h"
#include "../../Utils/Utils.h"

#include <algorithm>
#include <memory>
#include <stdexcept>
#include <string>

namespace Physics_NS {
    /// Medium type getter from MediumInterface pointer
    /// \param[in] medium MediumInterface pointer
    /// \return MediumType, MediumType::Unknown if cannot convert to the known types
    /// \throw std::invalid_argument if ENABLE_CHECK_CONTRACTS is defined and cannot convert to the known types
    template < typename T >
    inline MediumType mediumType(MediumInterface<T>* const medium) EXCEPT_INPUT_PARAMS;

    /// Medium type getter from MediumInterface pointer
    /// \param[in] medium Medium name
    /// \return MediumType, MediumType::Unknown if cannot convert to the known types
    /// \throw std::invalid_argument if ENABLE_CHECK_CONTRACTS is defined and cannot convert to the known types
    inline MediumType mediumType(const std::string& medium) EXCEPT_INPUT_PARAMS;

    /// Get medium type string representation
    /// \param[in] medium MediumType
    /// \return string representing medium type
    inline std::string to_string(const MediumType& medium) noexcept;

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
    if (dynamic_cast<MediumGlass<T>*>(medium))
        return MediumType::Glass;
    if (dynamic_cast<MediumConstant<T>*>(medium))
        return MediumType::Constant;
    if (dynamic_cast<MediumLinear<T>*>(medium))
        return MediumType::Linear;
    if (dynamic_cast<MediumArbitrary<T>*>(medium))
        return MediumType::Arbitrary;

    FAIL_ARGUMENT_CONTRACT("Medium type cannot be evaluated");

    return MediumType::Unknown;
}

Physics_NS::MediumType Physics_NS::mediumType(const std::string& medium) EXCEPT_INPUT_PARAMS {
    using namespace Utils_NS;

    auto lower = to_lower(medium);

    if (contains(getAllVariants(to_lower(MediumTypeStrings::Glass)), lower))
        return MediumType::Glass;
    if (contains(getAllVariants(to_lower(MediumTypeStrings::Constant)), lower))
        return MediumType::Constant;
    if (contains(getAllVariants(to_lower(MediumTypeStrings::Linear)), lower))
        return MediumType::Linear;
    if (contains(getAllVariants(to_lower(MediumTypeStrings::Arbitrary)), lower))
        return MediumType::Arbitrary;

    FAIL_ARGUMENT_CONTRACT("Medium type cannot be evaluated");

    return MediumType::Unknown;
}

std::string Physics_NS::to_string(const Physics_NS::MediumType& medium) noexcept {
    switch (medium) {
        case MediumType::Glass:
            return MediumTypeStrings::Glass;
        case MediumType::Constant:
            return MediumTypeStrings::Constant;
        case MediumType::Linear:
            return MediumTypeStrings::Linear;
        case MediumType::Arbitrary:
            return MediumTypeStrings::Arbitrary;
        default:
            return MediumTypeStrings::Unknown;
    }
}

template < typename T >
void Physics_NS::validate(const Physics_NS::MediumProperties<T>& properties) {
    CHECK_ARGUMENT_CONTRACT(properties.type != MediumType::Unknown);

    if (properties.type == MediumType::Glass) {
        CHECK_ARGUMENT_CONTRACT(properties.n0.has_value());
        CHECK_ARGUMENT_CONTRACT(properties.n0.value() >= 1);

        CHECK_ARGUMENT_CONTRACT(properties.r0.has_value());
        CHECK_ARGUMENT_CONTRACT(properties.r0.value() >= 0);

        CHECK_ARGUMENT_CONTRACT(properties.c0.has_value());
        CHECK_ARGUMENT_CONTRACT(properties.c0.value() >= 0);

        CHECK_ARGUMENT_CONTRACT(properties.k0.has_value());
        CHECK_ARGUMENT_CONTRACT(properties.k0.value() >= 0);
    }

    if (properties.type == MediumType::Constant) {
        CHECK_ARGUMENT_CONTRACT(properties.n0.has_value());
        CHECK_ARGUMENT_CONTRACT(properties.n0.value() >= 1);

        CHECK_ARGUMENT_CONTRACT(properties.a0.has_value());
        CHECK_ARGUMENT_CONTRACT(properties.a0.value() >= 0);

        CHECK_ARGUMENT_CONTRACT(properties.u0.has_value());
        CHECK_ARGUMENT_CONTRACT(properties.u0.value() >= 0);

        CHECK_ARGUMENT_CONTRACT(properties.g0.has_value());
        CHECK_ARGUMENT_CONTRACT(properties.g0.value() >= -1);
        CHECK_ARGUMENT_CONTRACT(properties.g0.value() <= +1);

        CHECK_ARGUMENT_CONTRACT(properties.r0.has_value());
        CHECK_ARGUMENT_CONTRACT(properties.r0.value() >= 0);

        CHECK_ARGUMENT_CONTRACT(properties.c0.has_value());
        CHECK_ARGUMENT_CONTRACT(properties.c0.value() >= 0);

        CHECK_ARGUMENT_CONTRACT(properties.k0.has_value());
        CHECK_ARGUMENT_CONTRACT(properties.k0.value() >= 0);
    }

    if (properties.type == MediumType::Linear) {
        CHECK_ARGUMENT_CONTRACT(properties.n0.has_value());
        CHECK_ARGUMENT_CONTRACT(properties.n0.value() >= 1);
        CHECK_ARGUMENT_CONTRACT(properties.nT.has_value());
        CHECK_ARGUMENT_CONTRACT(properties.nD.has_value());

        CHECK_ARGUMENT_CONTRACT(properties.a0.has_value());
        CHECK_ARGUMENT_CONTRACT(properties.a0.value() >= 0);
        CHECK_ARGUMENT_CONTRACT(properties.aT.has_value());
        CHECK_ARGUMENT_CONTRACT(properties.aD.has_value());

        CHECK_ARGUMENT_CONTRACT(properties.u0.has_value());
        CHECK_ARGUMENT_CONTRACT(properties.u0.value() >= 0);
        CHECK_ARGUMENT_CONTRACT(properties.uT.has_value());
        CHECK_ARGUMENT_CONTRACT(properties.uD.has_value());

        CHECK_ARGUMENT_CONTRACT(properties.g0.has_value());
        CHECK_ARGUMENT_CONTRACT(properties.g0.value() >= -1);
        CHECK_ARGUMENT_CONTRACT(properties.g0.value() <= +1);
        CHECK_ARGUMENT_CONTRACT(properties.gT.has_value());
        CHECK_ARGUMENT_CONTRACT(properties.gD.has_value());

        CHECK_ARGUMENT_CONTRACT(properties.r0.has_value());
        CHECK_ARGUMENT_CONTRACT(properties.r0.value() >= 0);
        CHECK_ARGUMENT_CONTRACT(properties.rT.has_value());
        CHECK_ARGUMENT_CONTRACT(properties.rD.has_value());

        CHECK_ARGUMENT_CONTRACT(properties.c0.has_value());
        CHECK_ARGUMENT_CONTRACT(properties.c0.value() >= 0);
        CHECK_ARGUMENT_CONTRACT(properties.cT.has_value());
        CHECK_ARGUMENT_CONTRACT(properties.cD.has_value());

        CHECK_ARGUMENT_CONTRACT(properties.k0.has_value());
        CHECK_ARGUMENT_CONTRACT(properties.k0.value() >= 0);
        CHECK_ARGUMENT_CONTRACT(properties.kT.has_value());
        CHECK_ARGUMENT_CONTRACT(properties.kD.has_value());
    }

    if (properties.type == MediumType::Arbitrary) {
        CHECK_ARGUMENT_CONTRACT(properties.n0.has_value());
        CHECK_ARGUMENT_CONTRACT(properties.n0.value() >= 1);
        CHECK_ARGUMENT_CONTRACT(properties.nT.has_value());
        CHECK_ARGUMENT_CONTRACT(properties.nD.has_value());
        CHECK_ARGUMENT_CONTRACT(properties.nDT.has_value());

        CHECK_ARGUMENT_CONTRACT(properties.a0.has_value());
        CHECK_ARGUMENT_CONTRACT(properties.a0.value() >= 0);
        CHECK_ARGUMENT_CONTRACT(properties.aT.has_value());
        CHECK_ARGUMENT_CONTRACT(properties.aD.has_value());
        CHECK_ARGUMENT_CONTRACT(properties.aDT.has_value());

        CHECK_ARGUMENT_CONTRACT(properties.u0.has_value());
        CHECK_ARGUMENT_CONTRACT(properties.u0.value() >= 0);
        CHECK_ARGUMENT_CONTRACT(properties.uT.has_value());
        CHECK_ARGUMENT_CONTRACT(properties.uD.has_value());
        CHECK_ARGUMENT_CONTRACT(properties.uDT.has_value());

        CHECK_ARGUMENT_CONTRACT(properties.g0.has_value());
        CHECK_ARGUMENT_CONTRACT(properties.g0.value() >= -1);
        CHECK_ARGUMENT_CONTRACT(properties.g0.value() <= +1);
        CHECK_ARGUMENT_CONTRACT(properties.gT.has_value());
        CHECK_ARGUMENT_CONTRACT(properties.gD.has_value());
        CHECK_ARGUMENT_CONTRACT(properties.gDT.has_value());

        CHECK_ARGUMENT_CONTRACT(properties.r0.has_value());
        CHECK_ARGUMENT_CONTRACT(properties.r0.value() >= 0);
        CHECK_ARGUMENT_CONTRACT(properties.rT.has_value());
        CHECK_ARGUMENT_CONTRACT(properties.rD.has_value());
        CHECK_ARGUMENT_CONTRACT(properties.rDT.has_value());

        CHECK_ARGUMENT_CONTRACT(properties.c0.has_value());
        CHECK_ARGUMENT_CONTRACT(properties.c0.value() >= 0);
        CHECK_ARGUMENT_CONTRACT(properties.cT.has_value());
        CHECK_ARGUMENT_CONTRACT(properties.cD.has_value());
        CHECK_ARGUMENT_CONTRACT(properties.cDT.has_value());

        CHECK_ARGUMENT_CONTRACT(properties.k0.has_value());
        CHECK_ARGUMENT_CONTRACT(properties.k0.value() >= 0);
        CHECK_ARGUMENT_CONTRACT(properties.kT.has_value());
        CHECK_ARGUMENT_CONTRACT(properties.kD.has_value());
        CHECK_ARGUMENT_CONTRACT(properties.kDT.has_value());
    }
}

template < typename T >
bool Physics_NS::validateSafe(const Physics_NS::MediumProperties<T>& properties) noexcept {
    try {
        validate(properties);
        return true;
    } catch(std::invalid_argument&) {
        return false;
    }
}

template < typename T >
Physics_NS::MediumProperties<T> Physics_NS::exportMediumProperties(Physics_NS::MediumInterface<T>* const medium) noexcept {
    MediumProperties<T> result;
    result.type = mediumType(medium);

    if (result.type == MediumType::Glass)
        if (auto casted_medium = dynamic_cast<MediumGlass<T>*>(medium)) {
            result.n0 = casted_medium->refraction          (0, 0);
            result.r0 = casted_medium->density             (0, 0);
            result.c0 = casted_medium->heat_capacity       (0, 0);
            result.k0 = casted_medium->thermal_conductivity(0, 0);
        }

    if (result.type == MediumType::Constant)
        if (auto casted_medium = dynamic_cast<MediumConstant<T>*>(medium)) {
            result.n0 = casted_medium->refraction          (0, 0);
            result.a0 = casted_medium->absorption          (0, 0);
            result.u0 = casted_medium->scattering          (0, 0);
            result.g0 = casted_medium->anisotropy          (0, 0);
            result.r0 = casted_medium->density             (0, 0);
            result.c0 = casted_medium->heat_capacity       (0, 0);
            result.k0 = casted_medium->thermal_conductivity(0, 0);
        }

    if (result.type == MediumType::Linear)
        if (auto casted_medium = dynamic_cast<MediumLinear<T>*>(medium)) {
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

            result.r0 = casted_medium->density(0, 0);
            result.rT = casted_medium->density(1, 0) - result.r0.value();
            result.rD = casted_medium->density(0, 1) - result.r0.value();

            result.c0 = casted_medium->heat_capacity(0, 0);
            result.cT = casted_medium->heat_capacity(1, 0) - result.c0.value();
            result.cD = casted_medium->heat_capacity(0, 1) - result.c0.value();

            result.k0 = casted_medium->thermal_conductivity(0, 0);
            result.kT = casted_medium->thermal_conductivity(1, 0) - result.k0.value();
            result.kD = casted_medium->thermal_conductivity(0, 1) - result.k0.value();
        }

    if (result.type == MediumType::Arbitrary)
        if (auto casted_medium = dynamic_cast<MediumArbitrary<T>*>(medium)) {
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

            result.r0  = casted_medium->density(0, 0);
            result.rT  = casted_medium->density(1, 0) - result.r0.value();
            result.rD  = casted_medium->density(0, 1) - result.r0.value();
            result.rDT = casted_medium->density(1, 1) - result.r0.value() - result.rT.value() - result.rD.value();

            result.c0  = casted_medium->heat_capacity(0, 0);
            result.cT  = casted_medium->heat_capacity(1, 0) - result.c0.value();
            result.cD  = casted_medium->heat_capacity(0, 1) - result.c0.value();
            result.cDT = casted_medium->heat_capacity(1, 1) - result.c0.value() - result.cT.value() - result.cD.value();

            result.k0  = casted_medium->thermal_conductivity(0, 0);
            result.kT  = casted_medium->thermal_conductivity(1, 0) - result.k0.value();
            result.kD  = casted_medium->thermal_conductivity(0, 1) - result.k0.value();
            result.kDT = casted_medium->thermal_conductivity(1, 1) - result.k0.value() - result.kT.value() - result.kD.value();
        }

    return result;
}
