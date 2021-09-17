#!/bin/bash
pvs-studio-analyzer trace -- g++ -std=c++2a -O2 -Iyaml-cpp/include -Ieigen -Igoogletest/googletest/include Tests/main.cpp -o Tests/unit_tests -pthread yaml-cpp/libyaml-cpp.a lib/libgtest.a lib/libgtest_main.a
pvs-studio-analyzer analyze -o log
plog-converter -a GA:1,2,3 -t fullhtml -o tasks log
