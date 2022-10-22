
aux_source_directory(. SRC_LIST)

# add the library
add_library(common ${SRC_LIST})

find_library(LIB_ZIP zip)
target_link_libraries(common "${LIB_ZIP}")

target_include_directories(common PUBLIC
                           "${PROJECT_BINARY_DIR}/include"
                           )
