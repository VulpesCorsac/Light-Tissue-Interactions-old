#pragma once

namespace MonteCarlo_NS {
    /// \brief Sweep for detectors distances
    template < typename T >
    struct DistancesSweep {
        const T from; ///< starting distance
        const T to;   ///< ending distance
        const T step; ///< step for distances sweep
    };
}
