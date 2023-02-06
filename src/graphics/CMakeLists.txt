cmake_minimum_required(VERSION 3.17.3 FATAL_ERROR)
project(graphics VERSION ${CMAKE_PROJECT_VERSION} LANGUAGES C CXX)
include(GNUInstallDirs)

add_library(${PROJECT_NAME} STATIC "")
target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> # or ${PROJECT_SOURCE_DIR}/include
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}> # or include
)
target_sources(${PROJECT_NAME} 
    PRIVATE
        glsl_shader.cpp
)

if (APPLE)
    INCLUDE_DIRECTORIES(/System/Library/Frameworks)
    FIND_LIBRARY(COCOA_LIBRARY Cocoa)
    FIND_LIBRARY(OpenGL_LIBRARY OpenGL)
    FIND_LIBRARY(IOKit_LIBRARY IOKit)
    FIND_LIBRARY(CoreVideo_LIBRARY CoreVideo)
    MARK_AS_ADVANCED(COCOA_LIBRARY OpenGL_LIBRARY)
    SET(APPLE_LIBS ${COCOA_LIBRARY} ${IOKit_LIBRARY} ${OpenGL_LIBRARY} ${CoreVideo_LIBRARY})
    SET(APPLE_LIBS ${APPLE_LIBS} ${GLFW3_LIBRARY} ${ASSIMP_LIBRARY})
    set(LIBS ${LIBS} ${APPLE_LIBS})
endif(APPLE)

if (APPLE)
    target_link_libraries(${PROJECT_NAME} 
        PUBLIC glfw ${LIBS} glad
    )
else()
    target_link_libraries(${PROJECT_NAME} 
        PUBLIC glfw glad
    )
endif()