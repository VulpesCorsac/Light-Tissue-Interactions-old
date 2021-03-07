add_executable(
    unit_tests
    main.cpp
    )

add_library(BugerTest.h INTERFACE)

target_link_libraries(
    unit_tests
    gtest_main
    BugerTest.h
    )

add_test(
  NAME
  TEST_BUGER_ADSORBTION
  COMMAND
    ${CMAKE_BINARY_DIR}/Tests/unit_tests
  )

