add_executable(
    unit_tests
    main.cpp
    )

add_library(tst_test_init.h INTERFACE)

target_link_libraries(
    unit_tests
    gtest_main
    tst_test_init.h
    )

add_test(
  NAME
  TEST_BUGER_ADSORBTION
  COMMAND
    ${CMAKE_BINARY_DIR}/mcml/tests/unit_tests
  )

