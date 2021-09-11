#!/bin/bash
pvs-studio-analyzer trace -- g++ -std=c++2a -O2 -Iyaml-cpp/include -Ieigen main_admc.cpp -o main_admc -pthread yaml-cpp/libyaml-cpp.a
pvs-studio-analyzer analyze -o log
plog-converter -a GA:1,2,3 -t fullhtml -o tasks log