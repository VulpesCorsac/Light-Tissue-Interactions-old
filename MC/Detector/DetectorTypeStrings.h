#pragma once

#include <string>

namespace MonteCarlo_NS {
    /// \brief supported Detector types
    class DetectorTypeStrings {
    public:
        inline static const std::string Unknown                  = "unknown"                   ; ///< unknown detector types e.g. interface
        inline static const std::string FullAbsorber             = "full_absorber"             ; ///< full absorbing detector, e.g. power meter
        inline static const std::string IntegratingSphereSimple  = "integrating_sphere_simple" ; ///< simple integrating sphere
        inline static const std::string IntegratingSphereComplex = "integrating_sphere_complex"; ///< integrating sphere with some additional properties
        inline static const std::string OpticalFiber             = "optical_fiber"             ; ///< optical fiber detector
    };
}
