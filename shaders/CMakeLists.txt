cmake_minimum_required(VERSION 3.16)

add_custom_target(shaders
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${SHADERS_SRC} ${SHADERS_DES}
    COMMENT "Copying ${SHADERS_SRC} to ${SHADERS_DES}"
    VERBATIM
)