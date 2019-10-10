cmake_minimum_required(VERSION 3.0)

project (main)

# compileflags
set(CMAKE_XSS_FLAGS "${CMAKE_XSS_FLAGS} - Wall -Wextra -Wpedantic -std=c++11")
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add files
include_directories(include)
file (GLOB_RECURSE PROJECT_SOURCES src/*.cpp)

# Organizing files
source_group ("sources" FILES ${PROJECT_SOURCES})

# set main as output executable
add_executable (${PROJECT_NAME} ${PROJECT_SOURCES})

# unittest-cpp
find_package(UnitTest++ REQUIRED)
include_directories(${UTPP_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} UnitTest++)

# Cscope integration
find_program(CSCOPE cscope)
if( CSCOPE )
    set(AllFiles ${PROJECT_SOURCES} ${PROJECT_HEADERS})
    string(REPLACE ";" "\n" AllFiles "${AllFiles}")
    set(CSCOPE_FILES ${CMAKE_BINARY_DIR}/cscope.files)
    set(CSCOPE_DB ${CMAKE_BINARY_DIR}/cscope.out)
    file(WRITE ${CSCOPE_FILES} "${AllFiles}")

    ADD_CUSTOM_COMMAND(
        OUTPUT ${CSCOPE_DB}
        COMMAND cd ${CMAKE_BINARY_DIR} && cscope -b -q
        DEPENDS ${CSCOPE_FILES}
    )

    ADD_CUSTOM_TARGET(gen_cscope_db ALL
        DEPENDS ${CSCOPE_DB}
    )
endif()
