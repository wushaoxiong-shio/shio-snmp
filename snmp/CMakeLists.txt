cmake_minimum_required(VERSION 3.27)

project(SHIO-SNMP)

include_directories(include)

set(SNMP_SRC
        "src/loger.cc")

if (NOT DEFINED LIBRARY_OUTPUT_PATH)
    set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
endif()

set(SNMP_DEPEND
        PRIVATE
        nlohmann_json::nlohmann_json
        pthread
        dl)

add_library(shio_snmp SHARED ${SNMP_SRC})
target_link_libraries(shio_snmp ${SNMP_DEPEND})