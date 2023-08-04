cmake_minimum_required(VERSION 3.27)

project(SHIO-SNMP)

set(CMAKE_C_FLAGS
        "$ENV{CFLAGS} \
        -std=c11 \
        -ggdb \
        -O0 \
        -rdynamic \
        -Wall \
        -Wno-deprecated \
        -Werror \
        -Wno-unused-function \
        -Wno-builtin-macro-redefined \
        -Wno-deprecated-declarations")

set(CMAKE_CXX_FLAGS
        "$ENV{CXXFLAGS} \
        -std=c++17 \
        -ggdb \
        -O0 \
        -rdynamic \
        -Wall \
        -Wno-deprecated \
        -Werror \
        -Wno-unused-function \
        -Wno-builtin-macro-redefined \
        -Wno-deprecated-declarations")

set(CMAKE_EXPORT_COMPILE_COMMANDS on)

add_executable(test_main tests/test_main.cc)