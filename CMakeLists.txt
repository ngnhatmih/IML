cmake_minimum_required(VERSION 3.16)

project(IML)

set(SOURCE_FILES 
    src/main.cpp
) 

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

set (IMGUI_SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/imgui)

add_subdirectory(${IMGUI_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui)