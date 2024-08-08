cmake_minimum_required(VERSION 3.16)

project(IML)

set(SOURCE_FILES 
    src/main.cpp
    src/theme.cpp
    src/game.cpp
) 

set(GLAD_DIR ${PROJECT_SOURCE_DIR}/vendor/glad)
set(IMGUI_DIR ${PROJECT_SOURCE_DIR}/vendor/imgui)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR/vendor/glm}
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

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
