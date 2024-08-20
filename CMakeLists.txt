cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(baskc)

include(FetchContent)
find_package(LLVM 14 REQUIRED CONFIG EXACT)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "-g")
endif()

set(CMAKE_CXX_FLAGS "-Wno-trigraphs -Wno-delete-abstract-non-virtual-dtor -Wno-return-type")

include_directories(include)
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

file(GLOB_RECURSE INCLUDES "include/*.h" "include/*.hpp")

# Find all the source files
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h" "src/*.hpp")

# Add the executable
add_executable(baskc src/main.cpp ${SOURCES} ${INCLUDES})
target_link_libraries(baskc LLVMSupport)

# Link against LLVM libraries
target_link_libraries(baskc LLVM)

# Add the test executable
file(GLOB_RECURSE TEST_SOURCES "test/*.cpp" "test/*.hpp" "test/*.h" "src/*.cpp" "src/*.h" "src/*.hpp")
list(FILTER TEST_SOURCES EXCLUDE REGEX "${CMAKE_SOURCE_DIR}/src/main.cpp")

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_definitions(-DTESTING)

add_executable(baskctest ${TEST_SOURCES} ${INCLUDES})

# Link against gtest
target_link_libraries(baskctest GTest::gtest_main)
# Link against LLVM libraries
target_link_libraries(baskctest LLVM)

target_compile_definitions(baskctest PRIVATE baskc_DEBUG)
target_link_libraries(baskctest LLVMSupport)

FILE(GENERATE OUTPUT ${CMAKE_BINARY_DIR}/TEST
     CONTENT 
"#!/usr/bin/env bash
# pass all arguments to the test script
echo 'Running unit tests'
./baskctest $@

echo 'Running test scripts'
../test/all.sh

"
     FILE_PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ)