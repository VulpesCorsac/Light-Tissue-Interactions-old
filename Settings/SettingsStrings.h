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

            /// string for Property_n0 node in yaml
            inline static const std::string N0 = "n0";
            /// string for Property_nT node in yaml
            inline static const std::string NT = "nT";
            /// string for Property_nD node in yaml
            inline static const std::string ND = "nD";
            /// string for Property_nDT node in yaml
            inline static const std::string NDT = "nDT";

            /// string for Property_a0 node in yaml
            inline static const std::string A0 = "a0";
            /// string for Property_aT node in yaml
            inline static const std::string AT = "aT";
            /// string for Property_aD node in yaml
            inline static const std::string AD = "aD";
            /// string for Property_aDT node in yaml
            inline static const std::string ADT = "aDT";

            /// string for Property_u0 node in yaml
            inline static const std::string U0 = "u0";
            /// string for Property_uT node in yaml
            inline static const std::string UT = "uT";
            /// string for Property_uD node in yaml
            inline static const std::string UD = "uD";
            /// string for Property_uDT node in yaml
            inline static const std::string UDT = "uDT";

            /// string for Property_g0 node in yaml
            inline static const std::string G0 = "g0";
            /// string for Property_gT node in yaml
            inline static const std::string GT = "gT";
            /// string for Property_gD node in yaml
            inline static const std::string GD = "gD";
            /// string for Property_gDT node in yaml
            inline static const std::string GDT = "gDT";
        };
    };
}
