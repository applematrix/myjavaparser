aux_source_directory(. SRC_LIST)

# add the library
add_library(instructions ${SRC_LIST})

target_include_directories(instructions PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          )

target_link_libraries(instructions PUBLIC classloader)
