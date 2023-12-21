cmake_minimum_required(VERSION 3.26)
project(projekt)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER "/usr/local/bin/g++")
include(FetchContent)

FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt
        GIT_TAG a0b8a92e3d1532361c2f7feb63babc5c18d00ef2
)
FetchContent_MakeAvailable(fmt)

add_executable(exec main.cpp Passwords.h Passwords.cpp Files.h Files.cpp Operations.h Operations.cpp UserInterface.cpp UserInterface.h)
target_link_libraries(exec fmt)