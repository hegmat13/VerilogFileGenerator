# CMakeList.txt : CMake project for dpgen, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.4)

set (CMAKE_CXX_STANDARD 11)
#SET(CMAKE_CXX_FLAGS "-Wall -Wextra")
SET(CMAKE_C_FLAGS "-Wall -Wextra")

file( GLOB SRCS *.cpp *.h)

add_executable( dpgen ${SRCS})

# TODO: Add tests and install targets if needed.
