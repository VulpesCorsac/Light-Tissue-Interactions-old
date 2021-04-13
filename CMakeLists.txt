cmake_minimum_required(VERSION 3.5)
project(MCML CXX)

add_executable(main_mcml main_mcml.cpp)
target_compile_features(main_mcml PUBLIC cxx_std_17)

add_executable(main_ad main_ad.cpp)
target_compile_features(main_ad PUBLIC cxx_std_17)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(main_mcml PRIVATE Threads::Threads)

add_subdirectory(googletest)

set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -O2 -std=c++17")

enable_testing()

add_subdirectory(MCML)
add_subdirectory(AD)
add_subdirectory(Tests)
