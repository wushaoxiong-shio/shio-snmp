cmake_minimum_required(VERSION 3.27)

project(SHIO-SNMP)

set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_BUILD_TYPE Release)

set(CMAKE_C_FLAGS_DEBUG
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

set(CMAKE_CXX_FLAGS_DEBUG
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

set(CMAKE_C_FLAGS_RELEASE "$ENV{CXXFLAGS} -std=c++17 -O3 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated-declarations")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -std=c++17 -O3 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated-declarations")

set(CMAKE_EXPORT_COMPILE_COMMANDS on)

find_package(nlohmann_json 3.2.7 REQUIRED)

include_directories(snmp/include)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_subdirectory(snmp)
link_directories(${PROJECT_SOURCE_DIR}/lib)
set(MAIN_DEPEND
        PRIVATE
        shio_snmp
        nlohmann_json::nlohmann_json)

add_executable(test_main tests/test_main.cc)
target_link_libraries(test_main PRIVATE ${MAIN_DEPEND})
