cmake_minimum_required(VERSION 3.16)
project(app)
 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXE_LINKER_FLAGS "-static")

file(GLOB_RECURSE SOURCES
    "${CMAKE_SOURCE_DIR}/*.cpp"
)
 
# Exclude the build directory
list(FILTER SOURCES EXCLUDE REGEX "${CMAKE_BINARY_DIR}/.*")
 
add_executable(app ${SOURCES})
 
# Output the binary to the project root, not inside build/
set_target_properties(app PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
)
 
target_include_directories(app PRIVATE .)
 
# Precompiled headers — heavy headers parsed once
target_precompile_headers(app PRIVATE
    <nlohmann/json.hpp>
    <string>
    <vector>
    <map>
    <iostream>
    <filesystem>
    <fstream>
    <algorithm>
    <atomic>
    <thread>
    <functional>
    <any>
)