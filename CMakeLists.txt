cmake_minimum_required(VERSION 3.0.0)

project(gr-render VERSION 0.1.0 LANGUAGES C CXX)

option(GR_COMPILE_LINUX "Compile for Linux" ON)
option(GR_COMPILE_STATIC_LIBRARY "Compile Static Library" OFF)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

set(render_src
    src/gVertexArray.cpp
    src/gTexture.cpp
    src/gRender.cpp
    src/gShader.cpp
    src/gl.cpp
)

set(render_hdr
    include/${PROJECT_NAME}/gShaderSource.h
    include/${PROJECT_NAME}/gVertexArray.h
    include/${PROJECT_NAME}/gVertex2D.h
    include/${PROJECT_NAME}/gVertex3D.h
    include/${PROJECT_NAME}/gTexture.h
    include/${PROJECT_NAME}/gRender.h
    include/${PROJECT_NAME}/gShader.h
    include/${PROJECT_NAME}/gCommon.h
    include/${PROJECT_NAME}/gColor.h
    include/${PROJECT_NAME}/gTypes.h
)

if(GR_COMPILE_STATIC_LIBRARY)
    add_library(${PROJECT_NAME} STATIC ${render_src} ${render_hdr})
else()
    add_library(${PROJECT_NAME} SHARED ${render_src} ${render_hdr})
endif()
unset(GR_COMPILE_STATIC_LIBRARY CACHE)

find_package(gr-math REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE ${GRMATH_LIBRARIES})

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}>
    $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
)

if(GR_COMPILE_LINUX)
    option(GR_USE_GLEW "Use glw" ON)

    if(GR_USE_GLEW)
        find_package(GLEW REQUIRED)

        target_link_libraries(${PROJECT_NAME} PRIVATE ${GLEW_LIBRARIES})
    endif()
    unset(GR_USE_GLEW CACHE)

    set(OpenGL_GL_PREFERENCE LEGACY)

    find_package(OpenGL REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_LIBRARIES})

else()
    find_library(EGL-lib EGL)
    find_library(GLESv3-lib GLESv3)

    target_link_libraries(${PROJECT_NAME} PRIVATE ${log-lib} ${android-lib} ${EGL-lib} ${GLESv3-lib})
endif()

unset(GR_COMPILE_LINUX CACHE)
