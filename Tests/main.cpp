// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "../Physics/AnglesTests.h"
#include "../Physics/BugerLambertTests.h"
#include "../Physics/ReflectanceTests.h"

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
