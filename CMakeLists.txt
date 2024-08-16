cmake_minimum_required(VERSION 3.16)

project(IML)

set(SOURCE_FILES 
    src/main.cpp
    src/theme.cpp
    src/game.cpp
    src/shader.cpp
)

set(GLAD_DIR ${PROJECT_SOURCE_DIR}/vendor/glad)
set(IMGUI_DIR ${PROJECT_SOURCE_DIR}/vendor/imgui)
set(STB_DIR ${PROJECT_SOURCE_DIR}/vendor/stb)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR/vendor/glm}
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

set(SHADERS_SRC ${CMAKE_SOURCE_DIR}/res/shaders)
set(SHADERS_DES ${CMAKE_BINARY_DIR}/res/shaders)

add_custom_target(shaders
    COMMAND ${CMAKE_COMMAND} -E $<IF:$<VERSION_LESS:${CMAKE_VERSION},3.26.0>,copy_directory,copy_directory_if_different> 
        ${SHADERS_SRC} 
        ${SHADERS_DES}
    COMMENT "Copying ${SHADERS_SRC} to ${SHADERS_DES}"
    VERBATIM
)

set(TEXTURE_SOURCE ${CMAKE_SOURCE_DIR}/res/textures)
set(TEXTURE_DES ${CMAKE_BINARY_DIR}/res/textures)

add_custom_target(textures
    COMMAND ${CMAKE_COMMAND} -E $<IF:$<VERSION_LESS:${CMAKE_VERSION},3.26.0>,copy_directory,copy_directory_if_different> 
        ${TEXTURE_SOURCE} 
        ${TEXTURE_DES}
    COMMENT "Copying ${TEXTURE_SOURCE} to ${TEXTURE_DES}"
    VERBATIM
)

if(WIN32) 
    target_link_libraries(IML PRIVATE dwmapi)
endif()

add_subdirectory(vendor/sdl)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)

add_subdirectory(${IMGUI_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui)

add_subdirectory(${GLAD_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE glad::glad)

add_subdirectory(vendor/glm)
target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)

add_subdirectory(vendor/stb)
target_link_libraries(${PROJECT_NAME} PRIVATE stb::stb)
