#include "TestBuger.h"
#include "TestAD.h"
#incldue "TestIAD.h"

#include <gtest/gtest.h>

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
