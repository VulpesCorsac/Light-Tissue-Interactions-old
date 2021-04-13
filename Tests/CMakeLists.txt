add_library(TestBuger.h INTERFACE)
add_library(TestAD.h INTERFACE)

add_executable(unit_tests main.cpp)

target_link_libraries(unit_tests gtest_main TestBuger.h TestAD.h)

add_test(NAME unit_tests COMMAND unit_tests)

