cmake_minimum_required(VERSION 3.4)

project(hlsyn)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS_DEBUG "-std=c++11")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-std=c++11")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-std=c++11")
set(CMAKE_CXX_FLAGS_RELEASE "-std=c++11")

add_subdirectory( src ) 