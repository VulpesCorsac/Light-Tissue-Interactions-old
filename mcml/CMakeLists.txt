add_executable(main main.cpp)
target_compile_features(main PUBLIC cxx_std_11)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(main PRIVATE Threads::Threads)

add_subdirectory(src/)
add_subdirectory(tests/)


