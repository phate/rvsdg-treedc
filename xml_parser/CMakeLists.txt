cmake_minimum_required(VERSION 3.13)

project (main CXX)

# compileflags
set(CMAKE_CSS_FLAGS "${CMAKE_CSS_FLAGS}" -Wall -Wextra -Wpedantic -std=c++17)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# import XML parser library: pugi
find_library(libpugixml_location NAMES libpugixml.a)
message(STATUS ${libpugixml_location})
add_library(libpugixml STATIC IMPORTED)
set_target_properties(libpugixml PROPERTIES IMPORTED_LOCATION ${libpugixml_location})

# Add files
include_directories(include)
file (GLOB_RECURSE PROJECT_SOURCES include/*.cpp)
file (GLOB_RECURSE PROJECT_SOURCES src/*.cpp)

# set main as output executable
add_compile_options(${CMAKE_CSS_FLAGS})
add_executable (${PROJECT_NAME} ${PROJECT_SOURCES})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

target_link_libraries(${PROJECT_NAME} libpugixml)

# add std::filesystem
target_link_libraries(${PROJECT_NAME} stdc++fs)
