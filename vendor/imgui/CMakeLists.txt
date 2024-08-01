cmake_minimum_required(VERSION 3.16)

project(imgui)

add_library(imgui STATIC
    ${IMGUI_SOURCE_DIR}/imgui.cpp
    ${IMGUI_SOURCE_DIR}/imgui.h
    ${IMGUI_SOURCE_DIR}/imconfig.h
    ${IMGUI_SOURCE_DIR}/imgui_demo.cpp
    ${IMGUI_SOURCE_DIR}/imgui_draw.cpp
    ${IMGUI_SOURCE_DIR}/imgui_internal.h
    ${IMGUI_SOURCE_DIR}/imgui_tables.cpp
    ${IMGUI_SOURCE_DIR}/imgui_widgets.cpp
    ${IMGUI_SOURCE_DIR}/imstb_rectpack.h
    ${IMGUI_SOURCE_DIR}/imstb_textedit.h
    ${IMGUI_SOURCE_DIR}/imstb_truetype.h
    ${IMGUI_SOURCE_DIR}/imgui_impl_sdl3.h
    ${IMGUI_SOURCE_DIR}/imgui_impl_sdl3.cpp
    ${IMGUI_SOURCE_DIR}/imgui_impl_sdlrenderer3.h
    ${IMGUI_SOURCE_DIR}/imgui_impl_sdlrenderer3.cpp
)

add_library(imgui::imgui ALIAS imgui)

target_include_directories(imgui PUBLIC ${IMGUI_SOURCE_DIR})
target_link_libraries(imgui PUBLIC SDL3::SDL3)