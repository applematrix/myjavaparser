cmake_minimum_required(VERSION 3.10)

# set the project name
project(myvm)

configure_file(MyVmConfig.h.in MyVmConfig.h)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(classloader)
add_subdirectory(instructions)
add_subdirectory(windows)
add_subdirectory(common)

list(APPEND EXTRA_LIBS classloader)
list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/classloader")
list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/common")

# add the executable
add_executable(myvm main.cpp)

target_link_libraries(myvm PUBLIC common classloader instructions windows pthread)

target_include_directories(myvm PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           ${EXTRA_INCLUDES}
                           )
