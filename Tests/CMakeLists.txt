add_library(BenchmarkHelper.h INTERFACE)

add_compile_definitions(ENABLE_CHECK_CONTRACTS)

file(GLOB sources ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
add_executable(unit_tests        ${sources})
#add_executable(unit_tests_asan ${sources})
#add_executable(unit_tests_tsan ${sources})
target_compile_features(unit_tests      PUBLIC cxx_std_17)
#target_compile_features(unit_tests_asan PUBLIC cxx_std_17)
#target_compile_features(unit_tests_tsan PUBLIC cxx_std_17)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

target_link_libraries(unit_tests      PRIVATE gtest_main yaml-cpp Threads::Threads)
#target_link_libraries(unit_tests_asan PRIVATE gtest_main yaml-cpp Threads::Threads)
#target_link_libraries(unit_tests_tsan PRIVATE gtest_main yaml-cpp Threads::Threads)

#target_compile_options(unit_tests_asan PRIVATE -fsanitize=address)
#target_link_options   (unit_tests_asan PRIVATE -fsanitize=address)
#target_compile_options(unit_tests_tsan PRIVATE -fsanitize=thread )
#target_link_options   (unit_tests_tsan PRIVATE -fsanitize=thread )

add_test(NAME unit_tests      COMMAND unit_tests     )
#add_test(NAME unit_tests_asan COMMAND unit_tests_asan)
#add_test(NAME unit_tests_tsan COMMAND unit_tests_tsan)
