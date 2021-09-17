#include "../Physics/Medium/MediumInterfaceTests.h"
#include "../Physics/Medium/MediumGlassTests.h"
#include "../Physics/Medium/MediumUtilsTests.h"

#include "TestAD.h"
#include "TestIAD.h"
#include "TestMC.h"

#include <gtest/gtest.h>

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
