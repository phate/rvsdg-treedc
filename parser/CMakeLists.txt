cmake_minimum_required(VERSION 3.0)

project (main)

# compileflags
set(CMAKE_XSS_FLAGS "${CMAKE_XSS_FLAGS} - Wall -Wextra -Wpedantic -std=c++11")
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
add_executable (${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_TESTS})

target_link_libraries(${PROJECT_NAME} libpugixml)
