add_executable(
    unit_tests
    tst_test_init.h
    )

target_link_libraries(
    unit_tests
    gtest_main
    )

add_test(
  NAME
  test
  COMMAND
    ${CMAKE_BINARY_DIR}/mcml/tests/unit_tests
  )
