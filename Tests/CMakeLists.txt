add_executable(unit_tests main.cpp)

add_library(TestBuger.h INTERFACE)
add_library(TestAD.h INTERFACE)

target_link_libraries(unit_tests gtest_main TestBuger.h TestAD.h)

add_test(
  NAME
  TEST_BUGER_ADSORBTION
  COMMAND
    ${CMAKE_BINARY_DIR}/Tests/unit_tests
  )

