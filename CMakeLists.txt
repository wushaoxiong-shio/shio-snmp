cmake_minimum_required(VERSION 3.27)

project(SHIO-SNMP)

set(CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} \
        -std=c++17 \
        -g \
        -O0")

set(CMAKE_EXPORT_COMPILE_COMMANDS on)

add_executable(test_main tests/test_main.cc)