cmake_minimum_required(VERSION 3.10)

# set the project name
project(myvm)

option(BUILD_DEBUG "Build project with dedug information" ON)

configure_file(MyVmConfig.h.in MyVmConfig.h)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (BUILD_DEBUG)
    SET(CMAKE_BUILD_TYPE "Debug")
    SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
    SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
endif()

add_subdirectory(classloader)
add_subdirectory(instructions)
add_subdirectory(windows)
add_subdirectory(common)

list(APPEND EXTRA_LIBS classloader)
list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/classloader")
list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/common")

# add the executable
add_executable(myvm main.cpp)

find_library(LIB_ZIP zip)
target_link_libraries(myvm PUBLIC common classloader instructions windows pthread "${LIB_ZIP}")

target_include_directories(myvm PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           ${EXTRA_INCLUDES}
                           )
