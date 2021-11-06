// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

// #define RUN_TEST_TOGETHER

#ifdef RUN_TEST_TOGETHER
    #include "../Utils/UtilsTests.h"
    #include "../Utils/StringUtilsTests.h"
	
    #include "../Physics/AnglesTests.h"
    #include "../Physics/BugerLambertTests.h"
    #include "../Physics/ReflectanceTests.h"

    #include "../Physics/Medium/MediumInterfaceTests.h"
    #include "../Physics/Medium/MediumGlassTests.h"
    #include "../Physics/Medium/MediumConstantTests.h"
    #include "../Physics/Medium/MediumLinearTests.h"
    #include "../Physics/Medium/MediumArbitraryTests.h"
    #include "../Physics/Medium/MediumUtilsTests.h"
    #include "../Physics/Medium/MediumFactoryTests.h"

    #include "../Physics/LayerTests.h"
    #include "../Physics/SpecimenTests.h"

    #include "../Math/Mesh3Tests.h"

    #include "../Settings/SettingsExporterLayerTests.h"
    #include "../Settings/SettingsExporterMediumTests.h"
    #include "../Settings/SettingsExporterSampleTests.h"
    #include "../Settings/SettingsImporterErrorsTests.h"
    #include "../Settings/SettingsImporterLayerTests.h"
    #include "../Settings/SettingsImporterMediumTests.h"
    #include "../Settings/SettingsImporterSampleTests.h"

	#include "../MC/Detector/DetectorInterfaceTests.h"
	#include "../MC/Detector/FullAbsorberTests.h"
	#include "../MC/Detector/IntegratingSphereSimple.h"
	#include "../MC/Detector/IntegratingSphereComplexTests.h"
	#include "../MC/Detector/OpticalFiberTests.h"
	#include "../MC/Detector/DetectorUtilsTests.h"
	#include "../MC/Detector/DetectorFactoryTests.h"

    #include "../AD/ADTests.h"
    #include "../AD/IADTests.h"

    #include "../MC/MCTests.h"
#endif // RUN_TEST_TOGETHER

#include <gtest/gtest.h>

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
