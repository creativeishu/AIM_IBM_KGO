cmake_minimum_required(VERSION 2.8)

project(AIM_IBM_KGO)

# Set a default build type for single-configuration
# CMake generators if no build type is set.
IF(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
   SET(CMAKE_BUILD_TYPE RelWithDebInfo)
ENDIF(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)

# Enable C++11 mode
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Enable helpful warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wredundant-decls -Woverloaded-virtual -Winit-self -Wunused -Wunused-function -Wshadow -Wwrite-strings -Wfloat-equal -Wconversion -pedantic -Wno-long-long")
# Disabled since triggered by thirdparty code
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wmissing-noreturn -Wold-style-cast")

set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELEASE} -g")
set(CMAKE_CXX_FLAGS_DEBUG "-ggdb -pg -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_OPENMP -fopenmp") # enable OpenMP

add_library(graph parse.cpp graph.cpp)

add_executable(parse_TEST
    parse_TEST.cpp
  )
target_link_libraries(parse_TEST graph)

add_executable(daemon
    daemon.cpp
  )
target_link_libraries(daemon graph)

add_executable(graph_test
    graph_test.cpp
  )
target_link_libraries(graph_test graph)

add_executable(graph_deterministic
    graph_deterministic.cpp
  )
target_link_libraries(graph_deterministic graph)
