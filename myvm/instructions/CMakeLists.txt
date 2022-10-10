aux_source_directory(. SRC_LIST)

# add the library
add_library(instructions ${SRC_LIST})

target_link_libraries(instructions PUBLIC classloader)
