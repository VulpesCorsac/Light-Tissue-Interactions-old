#include "AD/RT.h"
#include "AD/Quadrature.h"
#include "Tests/TestAD.h"
#include "Tests/TestIAD.h"


#include <cstdio>
#include <string>
#include <gtest/gtest.h>
#include <iostream>



int main (int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
