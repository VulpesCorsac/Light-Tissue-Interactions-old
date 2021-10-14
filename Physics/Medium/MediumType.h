#pragma once

namespace Physics_NS {
    /// \brief supported Medium types
    enum class MediumType {
        Unknown,  ///< unknown medium types e.g. interface
        Glass,    ///< glass medium
        Constant, ///< medium with constant properties
        Linear,   ///< medium with linear properties
        Arbitrary ///< medium with arbitrary properties
    };
}
