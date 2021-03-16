#include <gtest/gtest.h>

TEST(FirstTest, TestNumberOne) {
  EXPECT_NE(2, 1);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
