# MIT License
#
# Copyright (c) 2025 Vladislav Belousov
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

cmake_minimum_required(VERSION 3.20)
project(mint LANGUAGES C)

include(FetchContent)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

FetchContent_Declare(
        rtlib
        GIT_REPOSITORY https://github.com/vladbelousoff/rtlib.git
        GIT_TAG 3ce139cc582b2917529ca1fdd6abff32acd72b59
)
FetchContent_MakeAvailable(rtlib)

# Collect source files
file(GLOB_RECURSE SOURCE_FILES src/*.c)
file(GLOB_RECURSE HEADER_FILES include/*.h)

# Create mint-core library
add_library(mint-core STATIC ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(mint-core PUBLIC rtlib::rtlib)
target_compile_definitions(mint-core PUBLIC _CRT_SECURE_NO_WARNINGS)

# Specify include directories for mint-core
target_include_directories(mint-core PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Create the main executable
add_executable(mint mint.c)
target_link_libraries(mint PRIVATE mint-core)

# Testing executable (only built if this is the main project)
if (CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    add_executable(all_tests all_tests.c)

    target_link_libraries(all_tests PRIVATE mint-core)

    target_compile_definitions(all_tests PRIVATE
            MINT_TEST_DIR="${CMAKE_CURRENT_SOURCE_DIR}/tests"
    )

    # Enable testing
    enable_testing()

    # Add the test
    add_test(
            NAME all_tests
            COMMAND all_tests
    )
endif ()
