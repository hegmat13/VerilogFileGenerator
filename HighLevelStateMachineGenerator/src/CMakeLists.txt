set( CMAKE_C_FLAGS "-Wall -g" )

file( GLOB SRCS *.cpp *.hpp )

add_executable(hlsyn ${SRCS})