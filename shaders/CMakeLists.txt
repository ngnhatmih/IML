cmake_minimum_required(VERSION 3.16)

# add_custom_target never be up-to-date
# TODO: combine add_custom_target and add_custom_command to avoid rebuild (if add dependencies)
add_custom_target(shaders
    COMMAND ${CMAKE_COMMAND} -E $<IF:$<VERSION_LESS:${CMAKE_VERSION},3.26.0>,copy_directory,copy_directory_if_different> 
        ${SHADERS_SRC} 
        ${SHADERS_DES}
    COMMENT "Copying ${SHADERS_SRC} to ${SHADERS_DES}"
    VERBATIM
)