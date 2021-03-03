add_executable(main main.cpp)
target_compile_features(main PUBLIC cxx_std_11)

add_subdirectory(src/)
add_subdirectory(tests/)


