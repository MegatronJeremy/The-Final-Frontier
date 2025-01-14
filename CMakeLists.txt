cmake_minimum_required(VERSION 3.15)
project(TheFinalFrontier LANGUAGES C CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Source and header files
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.c")
file(GLOB_RECURSE HEADERS "inc/*.h")

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE inc)

# Compiler-specific options
if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX) # Enable warnings as errors
    target_compile_definitions(${PROJECT_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -march=native)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _GNU_SOURCE)
endif()

# Link options (if needed)
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_link_options(${PROJECT_NAME} PRIVATE -pthread)
endif()
