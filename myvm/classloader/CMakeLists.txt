
aux_source_directory(. SRC_LIST)

list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/common")

# add the library
add_library(classloader ${SRC_LIST})

target_include_directories(classloader PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           ${EXTRA_INCLUDES}
                           )