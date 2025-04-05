cmake_minimum_required(VERSION 3.30)
project(mint C)

include(FetchContent)

set(CMAKE_C_STANDARD 17)

FetchContent_Declare(
        rtlib
        GIT_REPOSITORY https://github.com/vladbelousoff/rtlib.git
        GIT_TAG 021cf137e823c9bd7952092dff52d59ed09660c3
)
FetchContent_MakeAvailable(rtlib)

add_executable(mint mint.c)
target_link_libraries(mint PRIVATE rtlib::rtlib)
