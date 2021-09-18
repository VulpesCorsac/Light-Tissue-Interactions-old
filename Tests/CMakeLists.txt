add_library(TestAD.h INTERFACE)
add_library(TestIAD.h INTERFACE)
add_library(TestMC.h INTERFACE)

add_executable(unit_tests main.cpp)
target_compile_features(unit_tests PUBLIC cxx_std_17)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(unit_tests PRIVATE gtest_main yaml-cpp Threads::Threads)

add_test(NAME unit_tests COMMAND unit_tests)

