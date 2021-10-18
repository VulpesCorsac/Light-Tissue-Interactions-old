#pragma once

#include <string>

namespace Physics_NS {
    /// \brief supported Medium types
    class MediumTypeStrings {
    public:
        inline static const std::string Unknown   = "unknown"  ; ///< unknown medium types e.g. interface
        inline static const std::string Glass     = "glass"    ; ///< glass medium
        inline static const std::string Constant  = "constant" ; ///< medium with constant properties
        inline static const std::string Linear    = "linear"   ; ///< medium with linear properties
        inline static const std::string Arbitrary = "arbitrary"; ///< medium with arbitrary properties
    };
}
