cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(KL)

include(FetchContent)
find_package(LLVM REQUIRED CONFIG)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "-g")
endif()

set(CMAKE_CXX_FLAGS "-Wno-trigraphs")

include_directories(include)
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

file(GLOB_RECURSE INCLUDES "include/*.h" "include/*.hpp")

# Find all the source files
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h" "src/*.hpp")

# Add the executable
add_executable(KL src/main.cpp ${SOURCES} ${INCLUDES})

# Link against LLVM libraries
target_link_libraries(KL LLVM)

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

add_executable(KLTest ${TEST_SOURCES} ${INCLUDES})

# Link against gtest
target_link_libraries(KLTest GTest::gtest_main)
# Link against LLVM libraries
target_link_libraries(KLTest LLVM)

target_compile_definitions(KLTest PRIVATE KL_DEBUG)
