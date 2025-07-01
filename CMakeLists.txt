cmake_minimum_required(VERSION 3.31)
project(LDM)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_executable(ldmi src/interface.cpp)
add_executable(ldml src/listener.cpp)
configure_file(${CMAKE_SOURCE_DIR}/src/daemon_cache.json ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/daemon_cache.json COPYONLY)