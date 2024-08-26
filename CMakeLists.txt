cmake_minimum_required(VERSION 3.14)

project(libevent-hpp)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

option(EVENTHPP_VENDORED "Build Libevent from local submodule" OFF)

file(GLOB EVENTHPP_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")

set(EVENTHPP_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(eventhpp STATIC ${EVENTHPP_SOURCES})

if(EVENTHPP_VENDORED)
    include(FetchContent)
    FetchContent_Declare(
    libevent
    GIT_REPOSITORY https://github.com/libevent/libevent.git
    GIT_TAG        master
    )
    option(EVENT__DISABLE_BENCHMARK "" ON)
    option(EVENT__DISABLE_TESTS "" ON)
    option(EVENT__DISABLE_SAMPLES "" ON)
    option(EVENT__DISABLE_REGRESS "" ON)
    FetchContent_Populate(libevent)
    add_subdirectory(${libevent_SOURCE_DIR})
    target_include_directories(
        eventhpp
        PUBLIC  ${EVENTHPP_INCLUDE_DIR})
    target_link_libraries(eventhpp event-static)
else()
    find_package(Libevent REQUIRED)
    target_include_directories(
        eventhpp
        PUBLIC  ${EVENTHPP_INCLUDE_DIR}
        PRIVATE ${LIBEVENT_INCLUDE_DIR})
endif()