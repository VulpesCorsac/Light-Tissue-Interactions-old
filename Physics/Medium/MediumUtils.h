#pragma once

#include "MediumInterface.h"
#include "MediumGlass.h"

#include "MediumType.h"

#include <stdexcept>

namespace Physics_NS {
    template < typename T >
    MediumType mediumType(MediumInterface<T>* medium);
}

/******************
 * IMPLEMENTATION *
 ******************/

template < typename T >
Physics_NS::MediumType Physics_NS::mediumType(Physics_NS::MediumInterface<T>* medium) {
    if (dynamic_cast<Physics_NS::MediumGlass<T>*>(medium))
        return Physics_NS::MediumType::Glass;

    throw std::invalid_argument("Medium type cannot be evaluated");
}
