cmake_minimum_required(VERSION 3.10)

# set the project name
project(myvm)

configure_file(MyVmConfig.h.in MyVmConfig.h)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(classloader)

list(APPEND EXTRA_LIBS classloader)
list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/classloader")
list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/common")

# add the executable
add_executable(myvm main.cpp)

target_link_libraries(myvm PUBLIC classloader)

target_include_directories(myvm PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           ${EXTRA_INCLUDES}
                           )