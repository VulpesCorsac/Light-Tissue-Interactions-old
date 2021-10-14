#pragma once

namespace MonteCarlo_NS {
    /// \brief supported Detector types
    enum class DetectorType {
        Unknown,                  ///< unknown detector types e.g. interface
        FullAbsorber,             ///< full absorbing detector, e.g. power meter
        IntegratingSphereSimple,  ///< simple integrating sphere
        IntegratingSphereComples, ///< integrating sphere with some additional properties
        OpticalFiber              ///< optical fiber detector
    };
}
