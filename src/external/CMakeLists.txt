cmake_minimum_required(VERSION 3.24)
include(FetchContent)
# # GLM Setup
add_subdirectory(glm)
# FetchContent_Declare(
#         glm 
#         GIT_REPOSITORY https://github.com/g-truc/glm.git 
#         GIT_TAG 0.9.9.8
# )
# FetchContent_MakeAvailable(glm)
# Glad Setup
# For Glad 2.0, use glad generator to create file and manually add it.
# Remove the following commands for glad v2.0
FetchContent_Declare(
        glad 
        GIT_REPOSITORY https://github.com/Dav1dde/glad.git 
        GIT_TAG v0.1.36
)
FetchContent_MakeAvailable(glad)
# GLFW Setup
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    glfw 
    GIT_REPOSITORY https://github.com/glfw/glfw.git 
    GIT_TAG 3.3.8
)
FetchContent_MakeAvailable(glfw)
# Imgui setup
FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG v1.89
)
FetchContent_GetProperties(imgui)
if (NOT imgui_POPULATED)
    FetchContent_Populate(imgui)

    set(IMGUI_INCLUDE_DIR ${imgui_SOURCE_DIR}/)
    file(GLOB IMGUI_SOURCES ${imgui_SOURCE_DIR}/*.cpp)
    file(GLOB IMGUI_HEADERS ${imgui_SOURCE_DIR}/*.h)

    add_library(imgui STATIC)
    target_sources( imgui
        PRIVATE
                ${imgui_SOURCE_DIR}/imgui_demo.cpp
                ${imgui_SOURCE_DIR}/imgui_draw.cpp
                ${imgui_SOURCE_DIR}/imgui_tables.cpp
                ${imgui_SOURCE_DIR}/imgui_widgets.cpp
                ${imgui_SOURCE_DIR}/imgui.cpp

        PRIVATE
                ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
                ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    )
    target_include_directories(
        imgui
        PUBLIC
        $<BUILD_INTERFACE:${IMGUI_INCLUDE_DIR}>
        $<BUILD_INTERFACE:${IMGUI_INCLUDE_DIR}/backends>
    )
    target_link_libraries(
        imgui
        PUBLIC
        glfw
    )
#     add_library(imgui STATIC ${IMGUI_SOURCES} ${IMGUI_SOURCES} ${imgui_SOURCE_DIR}/examples/imgui_impl_glfw.cpp ${imgui_SOURCE_DIR}/examples/imgui_impl_opengl3.cpp)
#     add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GLAD)
#     target_include_directories(imgui PUBLIC ${IMGUI_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} ${GLFW_INCLUDE_DIR} ${GLAD_INCLUDE_DIR})
#     target_link_libraries(imgui ${OPENGL_LIBRARIES} glfw glad)
endif ()

