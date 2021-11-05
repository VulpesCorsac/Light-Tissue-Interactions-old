#pragma once

#include <string>

namespace Settings_NS {
    class SettingsStrings {
    public:
        class Yaml {
        public:
            /// string for value node in yaml
            inline static const std::string Value = "value";
            /// string for description node in yaml
            inline static const std::string Description = "description";
        };

        class Medium {
        public:
            /// string for Type node in yaml
            inline static const std::string Type = "medium_type";

            /// string for property n0 node in yaml
            inline static const std::string N0 = "n0";
            /// string for property nT node in yaml
            inline static const std::string NT = "nT";
            /// string for property nD node in yaml
            inline static const std::string ND = "nD";
            /// string for property nDT node in yaml
            inline static const std::string NDT = "nDT";

            /// string for property a0 node in yaml
            inline static const std::string A0 = "a0";
            /// string for property aT node in yaml
            inline static const std::string AT = "aT";
            /// string for property aD node in yaml
            inline static const std::string AD = "aD";
            /// string for property aDT node in yaml
            inline static const std::string ADT = "aDT";

            /// string for property u0 node in yaml
            inline static const std::string U0 = "u0";
            /// string for property uT node in yaml
            inline static const std::string UT = "uT";
            /// string for property uD node in yaml
            inline static const std::string UD = "uD";
            /// string for property uDT node in yaml
            inline static const std::string UDT = "uDT";

            /// string for property g0 node in yaml
            inline static const std::string G0 = "g0";
            /// string for property gT node in yaml
            inline static const std::string GT = "gT";
            /// string for property gD node in yaml
            inline static const std::string GD = "gD";
            /// string for property gDT node in yaml
            inline static const std::string GDT = "gDT";

            /// string for property r0 node in yaml
            inline static const std::string R0 = "r0";
            /// string for property rT node in yaml
            inline static const std::string RT = "rT";
            /// string for property rD node in yaml
            inline static const std::string RD = "rD";
            /// string for property rDT node in yaml
            inline static const std::string RDT = "rDT";

            /// string for property c0 node in yaml
            inline static const std::string C0 = "c0";
            /// string for property cT node in yaml
            inline static const std::string CT = "cT";
            /// string for property cD node in yaml
            inline static const std::string CD = "cD";
            /// string for property cDT node in yaml
            inline static const std::string CDT = "cDT";

            /// string for property k0 node in yaml
            inline static const std::string K0 = "k0";
            /// string for property kT node in yaml
            inline static const std::string KT = "kT";
            /// string for property kD node in yaml
            inline static const std::string KD = "kD";
            /// string for property kDT node in yaml
            inline static const std::string KDT = "kDT";
        };

        class Layer {
        public:
            /// string for medium in layer
            inline static const std::string Medium = "medium";

            /// string for layer begin
            inline static const std::string Begin = "begin";
            /// string for layer end
            inline static const std::string End = "end";
            /// string for layer width
            inline static const std::string Width = "width";
        };
    };
}
