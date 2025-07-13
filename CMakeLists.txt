cmake_minimum_required(VERSION 3.10)

project(MovTix)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

set(SOURCES
    src/main.cpp
    src/auth.cpp
    src/booking.cpp
    src/movie.cpp
    src/ui.cpp
    src/utils.cpp
)

add_executable(MovTix ${SOURCES})