cmake_minimum_required(VERSION 3.31)
project(LDM)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_executable(LDM_INTERFACE src/interface.cpp)
add_executable(LDM_LISTENER src/listener.cpp)
file(COPY ${CMAKE_SOURCE_DIR}/src/daemon_cache.json DESTINATION ${CMAKE_SOURCE_DIR}/bin)